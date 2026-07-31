#include "../include/Camera.hpp"

Camera::Camera(float aspect) {
   
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    
    offset = glm::vec3(0.0f, 2.0f, -5.0f); 
    
    fov = glm::radians(60.0f); 
    aspectRatio = aspect;
    nearPlane = 0.1f;
    farPlane = 100.0f;
}

void Camera::updateChase(const glm::vec3& carPosition, const glm::vec3& carForward) {
    //inverse vector of car pointing
    glm::vec3 backward = -glm::normalize(carForward);
    
 
    position = carPosition + (backward * std::abs(offset.z)) + glm::vec3(0.0f, offset.y, 0.0f);
    
   
    target = carPosition;
}

glm::mat4 Camera::getViewMatrix() const {
   
    return glm::lookAt(position, target, up);
}

glm::mat4 Camera::getProjectionMatrix() const {
   
    return glm::perspective(fov, aspectRatio, nearPlane, farPlane);
}