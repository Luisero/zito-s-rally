#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class CameraMode { CHASE, FREE };

class Camera
{
public:
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
    glm::vec3 worldUp;
    glm::vec3 front;
    glm::vec3 right;

    glm::vec3 offset; // usado só no modo CHASE

    float fov;
    float aspectRatio;
    float nearPlane;
    float farPlane;

    float yaw;
    float pitch;
    float mouseSensitivity = 0.1f;
    float moveSpeed = 8.0f;

    CameraMode mode = CameraMode::CHASE;

    Camera(float aspect);

    void updateChase(const glm::vec3 &carPosition, const glm::vec3 &carForward, float deltaTime);

    // moveInput: x = strafe (direita/esquerda), y = subir/descer, z = frente/trás
    void updateFreeMove(float deltaTime, glm::vec3 moveInput);
    void processMouseMovement(float xOffset, float yOffset);
    void toggleMode();

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;

private:
    void updateFreeVectors();
};

#endif