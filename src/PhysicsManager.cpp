#include "../include/PhysicsManager.hpp"

PhysicsManager::PhysicsManager()
{
    RegisterDefaultAllocator();
    Trace = TraceImpl;
    JPH_IF_ENABLE_ASSERTS(AssertFailed = AssertFailedImpl;)
    Factory::sInstance = new Factory();

    RegisterTypes();
    // We need a temp allocator for temporary allocations during the physics update. We're
    // pre-allocating 10 MB to avoid having to do allocations during the physics update.
    // B.t.w. 10 MB is way too much for this example but it is a typical value you can use.
    // If you don't want to pre-allocate you can also use TempAllocatorMalloc to fall back to
    // malloc / free.
    temp_allocator = new TempAllocatorImpl(10 * 1024 * 1024);

    // We need a job system that will execute physics jobs on multiple threads. Typically
    // you would implement the JobSystem interface yourself and let Jolt Physics run on top
    // of your own job scheduler. JobSystemThreadPool is an example implementation.
    job_system = new JobSystemThreadPool(cMaxPhysicsJobs, cMaxPhysicsBarriers, thread::hardware_concurrency() - 1);

    broad_phase_layer_interface = new BPLayerInterfaceImpl();
    object_vs_broadphase_layer_filter = new ObjectVsBroadPhaseLayerFilterImpl();
    object_vs_object_layer_filter = new ObjectLayerPairFilterImpl();
    body_activation_listener = new MyBodyActivationListener();
    contact_listener = new MyContactListener();

    physics_system = new PhysicsSystem();

    physics_system->Init(cMaxBodies, cNumBodyMutexes, cMaxBodyPairs, cMaxContactConstraints, *broad_phase_layer_interface, *object_vs_broadphase_layer_filter, *object_vs_object_layer_filter);
    physics_system->SetBodyActivationListener(body_activation_listener);

    physics_system->SetContactListener(contact_listener);

    body_interface = &physics_system->GetBodyInterface();

    // CREATING FLOOR AND SPHERE TO TEST ===================================================
    BoxShapeSettings floor_shape_settings(Vec3(100.0f, 1.0f, 100.0f));
    floor_shape_settings.SetEmbedded();
    ShapeSettings::ShapeResult floor_shape_result = floor_shape_settings.Create();
    ShapeRefC floor_shape = floor_shape_result.Get(); // We don't expect an error here, but you can check floor_shape_result for HasError() / GetError()
    BodyCreationSettings floor_settings(floor_shape, RVec3(0.0_r, -1.0_r, 0.0_r), Quat::sIdentity(), EMotionType::Static, Layers::NON_MOVING);
    floor = body_interface->CreateBody(floor_settings);
    //body_interface->AddBody(floor->GetID(), EActivation::DontActivate);



    // --- CREATING SPHERE ---
    // 1. Criar o shape da esfera (Raio = 0.5f)
    SphereShapeSettings sphere_shape_settings(0.5f);
    ShapeSettings::ShapeResult sphere_shape_result = sphere_shape_settings.Create();
    ShapeRefC sphere_shape = sphere_shape_result.Get();

    // 2. Configurar a criação da esfera
    // Posição inicial: (0, 10, 0) - vai cair de 10 metros de altura
    // Tipo: Dynamic (se move com a física)
    // Layer: MOVING
    BodyCreationSettings sphere_settings(
        sphere_shape, 
        RVec3(0.0_r, 10.0_r, 0.0_r), 
        Quat::sIdentity(), 
        EMotionType::Dynamic, 
        Layers::MOVING
    );

    // 3. Fazer a bola quicar (Bounciness / Restitution)
    // O valor vai de 0.0 (não quica) a 1.0 (quica infinitamente sem perder energia)
    sphere_settings.mRestitution = 0.8f; 
    
    // (Opcional) Adicionar atrito
    sphere_settings.mFriction = 0.5f;

    // 4. Criar e adicionar o corpo
    sphere = body_interface->CreateBody(sphere_settings);
    
    // IMPORTANTE: Como é um corpo dinâmico, passe EActivation::Activate
    body_interface->AddBody(sphere->GetID(), EActivation::Activate);


    physics_system->OptimizeBroadPhase();
}

void PhysicsManager::update(float deltaTime)
{

    int collisionSteps = 1;
    if (deltaTime > 1.0f / 60.0f)
    {
        collisionSteps = static_cast<int>(deltaTime * 60.0f) + 1;
    }

    physics_system->Update(deltaTime, collisionSteps, temp_allocator, job_system);
   /*
   if (sphere != nullptr) {
    JPH::Vec3 position = body_interface->GetCenterOfMassPosition(sphere->GetID());
    
}
*/
}


JPH::BodyID PhysicsManager::createSphere(glm::vec3 position, float radius,
                                          EMotionType motionType, ObjectLayer layer,
                                          float restitution, float friction)
{
    SphereShapeSettings shapeSettings(radius);
    ShapeSettings::ShapeResult shapeResult = shapeSettings.Create();
    ShapeRefC shape = shapeResult.Get();

    BodyCreationSettings settings(
        shape,
        RVec3(position.x, position.y, position.z),
        Quat::sIdentity(),
        motionType,
        layer
    );
    settings.mRestitution = restitution;
    settings.mFriction = friction;

    Body* body = body_interface->CreateBody(settings);
    EActivation activation = (motionType == EMotionType::Static)
        ? EActivation::DontActivate
        : EActivation::Activate;
    body_interface->AddBody(body->GetID(), activation);

    return body->GetID();
}

JPH::BodyID PhysicsManager::createBox(glm::vec3 position, glm::vec3 halfExtents,
                                       EMotionType motionType, ObjectLayer layer,
                                       float restitution, float friction)
{
    BoxShapeSettings shapeSettings(Vec3(halfExtents.x, halfExtents.y, halfExtents.z));
    shapeSettings.SetEmbedded();
    ShapeSettings::ShapeResult shapeResult = shapeSettings.Create();
    ShapeRefC shape = shapeResult.Get();

    BodyCreationSettings settings(
        shape,
        RVec3(position.x, position.y, position.z),
        Quat::sIdentity(),
        motionType,
        layer
    );
    settings.mRestitution = restitution;
    settings.mFriction = friction;

    Body* body = body_interface->CreateBody(settings);
    EActivation activation = (motionType == EMotionType::Static)
        ? EActivation::DontActivate
        : EActivation::Activate;
    body_interface->AddBody(body->GetID(), activation);

    return body->GetID();
}