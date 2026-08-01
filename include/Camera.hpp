#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;

    glm::vec3 offset; // Distance to follow car

    float fov;
    float aspectRatio;
    float nearPlane;
    float farPlane;

    Camera(float aspect);

    void updateChase(const glm::vec3 &carPosition, const glm::vec3 &carForward, float deltaTime);
    
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
};

#endif