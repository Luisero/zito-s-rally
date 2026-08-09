#ifndef VEHICLE_HPP
#define VEHICLE_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Vehicle/VehicleConstraint.h>
#include <Jolt/Physics/Vehicle/WheeledVehicleController.h>
#include <Jolt/Physics/Vehicle/VehicleDifferential.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/OffsetCenterOfMassShape.h>
#include "./PhysicsManager.hpp"
#include "./Model.hpp"
#include "./Shader.hpp"

class Vehicle
{
public:
    Vehicle(PhysicsManager* physicsManager, glm::vec3 startPosition, Model* chassisModel);
    ~Vehicle();

    
    void setInput(float forward, float right, float brake, float handBrake);
    
   
    void draw(Shader& shader);

    JPH::BodyID getChassisId() const { return chassisId; }

private:
    PhysicsManager* physicsManager;
    JPH::VehicleConstraint* vehicleConstraint;
    JPH::BodyID chassisId;

    Model* chassisModel;
    Model* wheelModel;

    glm::mat4 JPHMat44ToGlm(const JPH::Mat44& joltMat);
};

#endif