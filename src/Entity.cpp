#include "../include/Entity.hpp"
#include "../include/Model.hpp"          
#include <Jolt/Physics/Body/BodyInterface.h>

glm::mat4 Entity::getWorldTransform(JPH::BodyInterface* bodyInterface) const
{
    if (!hasPhysics || bodyInterface == nullptr)
        return transform.toMatrix();

    JPH::RVec3 pos = bodyInterface->GetCenterOfMassPosition(bodyId);
    JPH::Quat rot = bodyInterface->GetRotation(bodyId);

    glm::mat4 t = glm::translate(glm::mat4(1.0f), glm::vec3(pos.GetX(), pos.GetY(), pos.GetZ()));
    glm::quat q(rot.GetW(), rot.GetX(), rot.GetY(), rot.GetZ());
    glm::mat4 r = glm::mat4_cast(q);

    return t * r * transform.toMatrix(); 
}

void Entity::draw(Shader &shader, JPH::BodyInterface* bodyInterface)
{
    if (model == nullptr)
        return;

    glm::mat4 world = getWorldTransform(bodyInterface);
    model->Draw(shader, world);
}