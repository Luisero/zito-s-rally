#ifndef GAMEPLAYSCENE_HPP
#define GAMEPLAYSCENE_HPP

#include "./Camera.hpp"
#include "./Entity.hpp"
#include "./Mesh.hpp"
#include "./Model.hpp"
#include "./PhysicsManager.hpp"
#include "./Scene.hpp"
#include "./Shader.hpp"
#include "./Skybox.hpp"
#include "./Vehicle.hpp"
#include "fmod_common.h"
#include "fmod_errors.h"
#include <fmod.hpp>
#include <fmod_errors.h>
#include <fmod_studio.hpp>
#include <glm/glm.hpp>
#include <vector>
class GameplayScene : public Scene {
public:
  ~GameplayScene() override;
  FMOD::Studio::System *fmodSystem = nullptr;
  FMOD::Studio::Bank *masterBank = nullptr;
  FMOD::Studio::Bank *stringsBank = nullptr;
  FMOD::Studio::EventDescription *engineDescription = nullptr;
  FMOD::Studio::EventInstance *engineInstance = nullptr;
  void setup() override;
  void handleEvent(sf::Event &event) override;
  void update(float deltaTime) override;
  void render() override;
  void refreshMouseState();

private:
  std::vector<Entity> entities;
  Model *car, *terrain;
  Mesh *triangle;
  Shader *shader, *skyBoxShader;
  Camera *camera;
  PhysicsManager *physicsManager;
  SkyBox *skyBox;
  Vehicle *vehicle;

  glm::vec3 carDummyPosition;
  glm::vec3 carDummyForward;
  glm::vec3 globalLightPos;
  float carSpeed = 0.0f;
  bool wireframeMode = false;
  int ticks = 0;
};

#endif
