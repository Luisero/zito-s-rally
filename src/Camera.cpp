#include "../include/Camera.hpp"

Camera::Camera(float aspect) {
  worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
  up = worldUp;

  offset = glm::vec3(0.0f, 2.0f, -10.0f);

  fov = glm::radians(60.0f);
  aspectRatio = aspect;
  nearPlane = 0.1f;
  farPlane = 100.0f;

  yaw = -90.0f;
  pitch = 0.0f;
  updateFreeVectors();

  position = glm::vec3(0.0f, 3.0f, -10.0f);
  target = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::updateFreeVectors() {
  glm::vec3 f;
  f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  f.y = sin(glm::radians(pitch));
  f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  front = glm::normalize(f);

  right = glm::normalize(glm::cross(front, worldUp));
  up = glm::normalize(glm::cross(right, front));
}

void Camera::processMouseMovement(float xOffset, float yOffset) {
  yaw += xOffset * mouseSensitivity;
  pitch += yOffset * mouseSensitivity;

  if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;

  updateFreeVectors();
}

void Camera::updateFreeMove(float deltaTime, glm::vec3 moveInput) {
  position += front * moveInput.z * moveSpeed * deltaTime;
  position += right * moveInput.x * moveSpeed * deltaTime;
  position += worldUp * moveInput.y * moveSpeed * deltaTime;
}

void Camera::toggleMode() {
  if (mode == CameraMode::CHASE) {
    mode = CameraMode::HOOD;
  } else if (mode == CameraMode::HOOD) {
    mode = CameraMode::FREE;

    // recalcula yaw/pitch a partir da direção atual, pra não "pular" a câmera
    // na troca
    glm::vec3 dir = glm::normalize(target - position);
    yaw = glm::degrees(atan2(dir.z, dir.x));
    pitch = glm::degrees(asin(dir.y));
    updateFreeVectors();
  } else // FREE
  {
    mode = CameraMode::CHASE;
  }
}

void Camera::updateChase(const glm::vec3 &carPosition,
                         const glm::vec3 &carForward, float deltaTime) {
  glm::vec3 backward = -glm::normalize(carForward);
  glm::vec3 desiredPosition = carPosition + (backward * std::abs(offset.z)) +
                              glm::vec3(0.0f, offset.y, 0.0f);
  glm::vec3 desiredTarget = carPosition;

  float positionLerpSpeed = 5.0f;
  float targetLerpSpeed = 10.0f;

  position = glm::mix(position, desiredPosition, positionLerpSpeed * deltaTime);
  target = glm::mix(target, desiredTarget, targetLerpSpeed * deltaTime);
}

void Camera::updateHood(const glm::vec3 &carPosition,
                        const glm::vec3 &carForward, const glm::vec3 &carUp) {
  glm::vec3 fwd = glm::normalize(carForward);
  glm::vec3 upDir = glm::normalize(carUp);
  glm::vec3 rightDir = glm::normalize(glm::cross(fwd, upDir));

  // posição rígida: soldada no carro, sem lerp, pra acompanhar exatamente a
  // inclinação/pitch/roll do chassi
  position = carPosition + rightDir * hoodOffset.x + upDir * hoodOffset.y +
             fwd * hoodOffset.z;

  target = position + fwd;
  up = upDir;
}

glm::mat4 Camera::getViewMatrix() const {
  if (mode == CameraMode::FREE)
    return glm::lookAt(position, position + front, up);

  return glm::lookAt(position, target, up);
}

glm::mat4 Camera::getProjectionMatrix() const {
  return glm::perspective(fov, aspectRatio, nearPlane, farPlane);
}
