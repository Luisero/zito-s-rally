#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyInterface.h>
#include "./Shader.hpp"
#include "./Transform.hpp"

class Model;                          // forward declaration — Entity só guarda ponteiro
namespace JPH { class BodyInterface; } // idem, evita puxar o header pesado do Jolt aqui

class Entity
{
public:
    Model* model = nullptr; 

    Transform transform;
    bool hasPhysics = false;
    JPH::BodyID bodyId; 

    glm::mat4 getWorldTransform(JPH::BodyInterface* bodyInterface) const;
    void draw(Shader &shader, JPH::BodyInterface* bodyInterface);
};

#endif