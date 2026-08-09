#include "../include/Vehicle.hpp"

glm::mat4 Vehicle::JPHMat44ToGlm(const JPH::Mat44 &joltMat) {
  glm::mat4 glmMat;

  joltMat.StoreFloat4x4(reinterpret_cast<JPH::Float4 *>(&glmMat[0][0]));
  return glmMat;
}

Vehicle::Vehicle(PhysicsManager *physicsManager, glm::vec3 startPos,
                 Model *chassisModel) {
  this->physicsManager = physicsManager;
  this->chassisModel = chassisModel;
  this->wheelModel = nullptr;
  Vec3 chassis_half_extents(1.0f, 0.4f, 2.0f);

  Ref<Shape> box_shape = new BoxShape(chassis_half_extents);
  // Offset the center of mass downward (-0.3f on Y axis)
  OffsetCenterOfMassShapeSettings com_settings(Vec3(0.0f, -0.3f, 0.0f),
                                               box_shape);
  Shape::ShapeResult shape_result = com_settings.Create();
  Ref<Shape> chassis_shape = shape_result.Get();

  // 2. Create the Chassis Rigid Body
  BodyCreationSettings chassis_settings(
      chassis_shape, Vec3(startPos.x, startPos.y, startPos.z),
      Quat::sIdentity(), EMotionType::Dynamic, Layers::MOVING);
  chassis_settings.mMassPropertiesOverride.mMass = 850.0f; // 1500 kg
  chassis_settings.mOverrideMassProperties =
      EOverrideMassProperties::CalculateInertia;

  BodyInterface &body_interface =
      physicsManager->physics_system->GetBodyInterface();
  chassisId =
      body_interface.CreateAndAddBody(chassis_settings, EActivation::Activate);
  this->chassisId = chassisId;
  // 3. Define the Vehicle Constraint Settings
  VehicleConstraintSettings vehicle_settings;
  vehicle_settings.mDrawConstraintSize = 0.1f;
  vehicle_settings.mMaxPitchRollAngle = DegreesToRadians(60.0f);

  // Wheel dimensions
  float wheel_radius = 0.4f;
  float wheel_width = 0.3f;
  float half_vehicle_width = 0.9f;
  float half_vehicle_length = 1.5f;

  // Configure 4 identical wheels
  for (int i = 0; i < 4; ++i) {
    bool is_front = (i < 2);
    bool is_left = (i % 2 == 0);

    WheelSettingsWV *wheel = new WheelSettingsWV();
    wheel->mRadius = wheel_radius;
    wheel->mWidth = wheel_width;

    // Position relative to chassis center
    wheel->mPosition =
        Vec3(is_left ? -half_vehicle_width : half_vehicle_width, -0.2f,
             is_front ? half_vehicle_length : -half_vehicle_length);

    // Suspension properties
    wheel->mSuspensionDirection = Vec3(0, -1, 0);
    wheel->mSuspensionMinLength = 0.0f;
    wheel->mSuspensionMaxLength = 0.5f;
    wheel->mSuspensionSpring.mFrequency = 2.0f; // Hardness
    wheel->mSuspensionSpring.mDamping = 0.5f;   // Damping ratio

    // Steering & Traction settings
    wheel->mMaxSteerAngle = is_front ? DegreesToRadians(35.0f) : 0.0f;

    wheel->mMaxBrakeTorque = 2800.0f;

    // O freio de mão atua APENAS nas traseiras
    wheel->mMaxHandBrakeTorque = is_front ? 0.0f : 8000.0f;

    wheel->mLateralFriction.Clear();
    wheel->mLateralFriction.AddPoint(.0f, 1.f);
    wheel->mLateralFriction.AddPoint(3.f, 1.0f);
    wheel->mLateralFriction.AddPoint(10.f, 0.6f);
    wheel->mLateralFriction.AddPoint(30.f, 0.5f);

    wheel->mLongitudinalFriction.Clear();
    wheel->mLongitudinalFriction.AddPoint(0.f, 0.f);
    wheel->mLongitudinalFriction.AddPoint(.2f, 1.f);
    wheel->mLongitudinalFriction.AddPoint(.6f, .6f);
    wheel->mLongitudinalFriction.AddPoint(1.f, 0.5f);

    vehicle_settings.mWheels.push_back(wheel);
  }

  // 4. Setup Motor, Differential, and Transmission Controllers
  WheeledVehicleControllerSettings *controller_settings =
      new WheeledVehicleControllerSettings();
  vehicle_settings.mController = controller_settings;

  // Engine settings
  controller_settings->mEngine.mMaxTorque = 400.0f; // Nm
  controller_settings->mEngine.mMaxRPM = 6800.0f;

  controller_settings->mTransmission.mMode = JPH::ETransmissionMode::Auto;
  controller_settings->mTransmission.mGearRatios = {2.66f, 1.78f, 1.3f, 1.0f,
                                                    0.74f};

  // Differentials (Distribute split power 50/50 between left/right)
  VehicleDifferentialSettings front_diff;
  front_diff.mLeftWheel = 0;
  front_diff.mRightWheel = 1;

  VehicleDifferentialSettings rear_diff;
  rear_diff.mLeftWheel = 2;
  rear_diff.mRightWheel = 3;

  controller_settings->mDifferentials.push_back(front_diff);
  // controller_settings->mDifferentials.push_back(rear_diff);

  // 5. Construct and Bind Constraint to World
  JPH::BodyLockWrite lock(
      physicsManager->physics_system->GetBodyLockInterface(), chassisId);
  if (lock.Succeeded()) {
    JPH::Body &chassis = lock.GetBody();
    vehicleConstraint = new JPH::VehicleConstraint(chassis, vehicle_settings);

    // vehicleConstraint->SetVehicleCollisionTester(new
    // VehicleCollisionTesterRaycast(Layers::MOVING));
    vehicleConstraint->SetVehicleCollisionTester(
        new VehicleCollisionTesterRay(Layers::MOVING));

    physicsManager->physics_system->AddConstraint(vehicleConstraint);
    physicsManager->physics_system->AddStepListener(vehicleConstraint);
  }

  // Use standard raycast tester to check ground surface collisions
}

void Vehicle::setInput(float forward, float right, float brake,
                       float handBrake) {
  JPH::BodyLockWrite lock(
      physicsManager->physics_system->GetBodyLockInterface(), chassisId);
  if (lock.Succeeded()) {
    JPH::Body &chassis = lock.GetBody();
    WheeledVehicleController *controller =
        static_cast<WheeledVehicleController *>(
            vehicleConstraint->GetController());

    controller->SetDriverInput(forward, right, brake, handBrake);
  }
}

void Vehicle::draw(Shader &shader) {
  if (chassisModel == nullptr)
    return;

  JPH::Mat44 joltTransform =
      physicsManager->physics_system->GetBodyInterface().GetWorldTransform(
          chassisId);

  glm::mat4 modelMatrix = JPHMat44ToGlm(joltTransform);

  modelMatrix = glm::scale(modelMatrix, glm::vec3(0.9f));
  modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -1.3f, 0.f));
  chassisModel->Draw(shader, modelMatrix);
}

int Vehicle::getCurrentGear() const {
  if (vehicleConstraint) {
    JPH::WheeledVehicleController *controller =
        static_cast<JPH::WheeledVehicleController *>(
            vehicleConstraint->GetController());
    return controller->GetTransmission().GetCurrentGear();
  }
  return 0;
}

float Vehicle::getCurrentRPM() const {
  if (vehicleConstraint) {
    JPH::WheeledVehicleController *controller =
        static_cast<JPH::WheeledVehicleController *>(
            vehicleConstraint->GetController());
    return controller->GetEngine().GetCurrentRPM();
  }
  return 0.0f;
}
