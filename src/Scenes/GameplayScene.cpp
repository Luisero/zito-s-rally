#include "../include/GameplayScene.hpp"
#include "../include/Game.hpp"
#include "Camera.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "fmod_common.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

void checkFmodError(FMOD_RESULT result) {
  if (result != FMOD_OK) {
    std::cerr << "[ERRO] FMOD: " << FMOD_ErrorString(result) << std::endl;
  }
}

GameplayScene::~GameplayScene() {
  delete physicsManager;
  delete shader;
  delete skyBoxShader;
  delete camera;
  delete skyBox;
  delete car;
  delete terrain;
}
float shootCooldown = 0.0f;
void GameplayScene::setup() {

  game->window.pushGLStates();
  sf::Texture loadingTex;
  if (loadingTex.loadFromFile("./assets/Textures/loading.png")) {
    sf::Sprite loadingSprite(loadingTex);
    loadingSprite.setScale(sf::Vector2(.7f, .7f));
    loadingSprite.setPosition(0, -10);
    game->window.draw(loadingSprite);
  }
  game->window.popGLStates();

  game->window.display();
  physicsManager = new PhysicsManager();

  game->assetsManager->loadImage("./assets/Textures/dirt_ground.jpeg",
                                 "dirt_ground");
  game->assetsManager->loadImage("./assets/Textures/cobblestone.jpg", "cobble");
  game->assetsManager->loadImage("./assets/Textures/grass.jpg", "grass");
  game->assetsManager->loadImage("./assets/Textures/red_checker.png",
                                 "checker");

  sf::Music music;

  // Load the song from file
  if (!music.openFromFile("assets/Songs/Colin McRae Rally 2 in-Game 3 - "
                          "Jonathan Colling (youtube).mp3")) {
    std::cout << "ERROR loading song";
  }
  music.setVolume(40);
  // Play the song
  music.play();

  // Loop the music (optional)
  music.setLoop(true);

  //-------------- SETUP FMOD
  FMOD_RESULT result = FMOD::Studio::System::create(&fmodSystem);
  checkFmodError(result);
  if (fmodSystem) {
    result = fmodSystem->initialize(512, FMOD_STUDIO_INIT_NORMAL,
                                    FMOD_INIT_NORMAL, nullptr);
    checkFmodError(result);

    if (result == FMOD_OK) {
      std::cout << "FMOD succesfully loaded!" << std::endl;
    }
  }
  FMOD_RESULT r1 = fmodSystem->loadBankFile(
      "./assets/fmod/Master.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank);
  checkFmodError(r1);
  FMOD_RESULT r2 =
      fmodSystem->loadBankFile("./assets/fmod/Master.strings.bank",
                               FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank);
  checkFmodError(r2);

  FMOD_RESULT r3 = fmodSystem->getEvent("event:/Engine", &engineDescription);
  checkFmodError(r3);
  engineDescription->createInstance(&engineInstance);
  engineInstance->start();
  car = game->assetsManager->loadModel("./assets/Models/wheelesscar/scene.gltf",
                                       "car");
  Model *wheelModel = game->assetsManager->loadModel(
      "assets/Models/jaguar_20_spoke_wheel/scene.gltf", "wheelModel");
  terrain = new Model(
      "./assets/Models/lil_cow_-_harvest_moon_back_to_nature/scene.gltf",
      game->assetsManager);

  shader = new Shader("./assets/Shaders/default.vert.glsl",
                      "./assets/Shaders/default.frag.glsl");
  skyBoxShader = new Shader("./assets/Shaders/skybox.vert.glsl",
                            "./assets/Shaders/skybox.frag.glsl");

  skyBox = new SkyBox(std::string("./assets/SkyBoxes/SkyBox/"));

  vehicle =
      new Vehicle(physicsManager, glm::vec3(0.f, 10.f, 0.f), car, wheelModel);

  globalLightPos = glm::vec3(2.f, 4.0f, 0.0f);
  camera = new Camera(1280.0f / 720.0f);

  carDummyPosition = glm::vec3(0.0f, 1.0f, 0.0f);
  carDummyForward = glm::vec3(0.0f, 0.0f, 1.0f);

  /*Model *floorModel = game->assetsManager->loadModel(
      "./assets/Models/checkered_tile_floor/scene.gltf", "floor");
  JPH::BodyID floorId = physicsManager->createBox(
      glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1000.0f, 1.0f, 1000.0f),
      JPH::EMotionType::Static, Layers::NON_MOVING);
  Entity floorEntity;
  floorEntity.model = floorModel;
  floorEntity.hasPhysics = true;
  floorEntity.bodyId = floorId;
  floorEntity.transform.position = glm::vec3(0, -1.f, 0.);
  floorEntity.transform.scale = glm::vec3(.5f, .5f, .5f);
  //entities.push_back(floorEntity);*/

  Model *ballModel = game->assetsManager->loadModel(
      "./assets/Models/sphere___low_poly/scene.gltf", "ball");
  Model *boxModel =
      game->assetsManager->loadModel("./assets/Models/Crate/Crate1.3ds", "box");

  float physicsRadius = 0.4f;
  float ballScale = physicsRadius / ballModel->getBoundingRadius();

  for (int i = -10; i < 10; i += 2) {
    JPH::BodyID sphereId = physicsManager->createSphere(
        glm::vec3(-((float)i) * 1.5, 20.0f, 0.f), physicsRadius,
        JPH::EMotionType::Dynamic, Layers::MOVING, 0.8f, 0.5f);

    Entity ballEntity;
    ballEntity.model = ballModel;
    ballEntity.hasPhysics = true;
    ballEntity.bodyId = sphereId;
    ballEntity.transform.scale = glm::vec3(ballScale * .9f);
    entities.push_back(ballEntity);

    for (int j = 0; j < 40; j += 2) {

      glm::vec3 boxSize(0.5f, 0.5f, 0.5f);

      JPH::BodyID boxId = physicsManager->createBox(
          glm::vec3((float)j, 10.f + i, (float)i), boxSize,
          JPH::EMotionType::Dynamic, Layers::MOVING);

      Entity boxEntity;
      boxEntity.model = boxModel;
      boxEntity.hasPhysics = true;
      boxEntity.bodyId = boxId;

      boxEntity.transform.scale = boxSize;

      entities.push_back(boxEntity);
    }
  }

  Model *trackModel = game->assetsManager->loadModel(
      "./assets/Models/sacoverde_gltf/sacoverde.gltf", "track");

  JPH::BodyID trackCollisionId = physicsManager->createMeshBody( 
      trackModel, glm::vec3(0.0f, -10.0f, 0.0f), 0.8f);

  Entity trackEntity;
  trackEntity.model = trackModel;
  trackEntity.hasPhysics = true;
  trackEntity.bodyId = trackCollisionId;

  entities.push_back(trackEntity);
}

void GameplayScene::handleEvent(sf::Event &event) {
  if (event.type == sf::Event::KeyPressed &&
      event.key.code == sf::Keyboard::C) {
    camera->toggleMode();
    bool freeMode = (camera->mode == CameraMode::FREE);

    game->window.setMouseCursorVisible(!freeMode);
    game->window.setMouseCursorGrabbed(freeMode);

    if (freeMode) {
      sf::Vector2i center((int)game->window.getSize().x / 2,
                          (int)game->window.getSize().y / 2);
      sf::Mouse::setPosition(center, game->window);
    }
  }
  if (event.type == sf::Event::KeyPressed &&
      event.key.code == sf::Keyboard::H) {
    camera->mode = CameraMode::HOOD;
  }

  if (event.type == sf::Event::KeyPressed &&
      event.key.code == sf::Keyboard::Escape) {
    game->changeState(GameState::MENU);
  }

  if (sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
      camera->mode == CameraMode::FREE) {
    if (shootCooldown <= 0.0f) {
      Model *ballModel = game->assetsManager->getModel("ball");

      float physicsRadius = 0.4f;
      float ballScale = physicsRadius / ballModel->getBoundingRadius();

      JPH::BodyID sphereId = physicsManager->createSphere(
          camera->position, physicsRadius, JPH::EMotionType::Dynamic,
          Layers::MOVING, 0.8f, 0.5f);

      glm::vec3 shootDirection = camera->front;

      float shootSpeed = 40.0f;

      physicsManager->body_interface->SetLinearVelocity(
          sphereId, JPH::Vec3(shootDirection.x * shootSpeed,
                              shootDirection.y * shootSpeed,
                              shootDirection.z * shootSpeed));

      Entity ballEntity;
      ballEntity.model = ballModel;
      ballEntity.hasPhysics = true;
      ballEntity.bodyId = sphereId;
      ballEntity.transform.scale = glm::vec3(ballScale * .9f);

      entities.push_back(ballEntity);

      shootCooldown = 0.5f;
    }
  }
}

void GameplayScene::update(float deltaTime) {
  ticks++;
  physicsManager->update(deltaTime);
  float zdistance = camera->offset.z;
  JPH::Vec3 joltCarPos =
      physicsManager->body_interface->GetPosition(vehicle->getChassisId());

  JPH::Mat44 joltCarTransform =
      physicsManager->body_interface->GetWorldTransform(
          vehicle->getChassisId());

  JPH::Vec3 joltCarFwd = joltCarTransform.GetAxisZ();
  JPH::Vec3 joltCarUp = joltCarTransform.GetAxisY();

  carDummyPosition =
      glm::vec3(joltCarPos.GetX(), joltCarPos.GetY(), joltCarPos.GetZ());

  carDummyForward = glm::normalize(
      glm::vec3(joltCarFwd.GetX(), joltCarFwd.GetY(), joltCarFwd.GetZ()));
  glm::vec3 carDummyUp = glm::normalize(
      glm::vec3(joltCarUp.GetX(), joltCarUp.GetY(), joltCarUp.GetZ()));
  if (shootCooldown > 0.0f) {
    shootCooldown -= deltaTime;
  }

  if (fmodSystem) {
    fmodSystem->update();
  }
  // camera->toggleMode();
  // bool freeMode = (camera->mode == CameraMode::FREE);

  ImGui::Begin("Zito-s-rally control panel");
  ImGui::Text("FPS: %.1f", 1.0f / deltaTime);
  ImGui::SliderFloat("Camera distance", &zdistance, 3.0f, 10.0f);
  ImGui::Text("Position: X:%.2f Y:%.2f Z:%.2f", carDummyPosition.x,
              carDummyPosition.y, carDummyPosition.z);
  if (ImGui::Button("Reset Position"))
    carDummyPosition = glm::vec3(0.0f, 1.0f, 0.0f);
  ImGui::Checkbox("Wireframe mode: ", &wireframeMode);
  ImGui::Text("Camera: %s (C to change, ESC to menu)",
              camera->mode == CameraMode::FREE ? "Free" : "Chasing car");

  ImGui::End();
  if (sf::Joystick::isConnected(0)) {
    ImGui::Begin("Joystick Debug");
    ImGui::Text("Eixo X (Analogico Esq): %.1f",
                sf::Joystick::getAxisPosition(0, sf::Joystick::X));
    ImGui::Text("Eixo Y: %.1f",
                sf::Joystick::getAxisPosition(0, sf::Joystick::Y));
    ImGui::Text("Eixo Z: %.1f",
                sf::Joystick::getAxisPosition(0, sf::Joystick::Z));
    ImGui::Text("Eixo R: %.1f",
                sf::Joystick::getAxisPosition(0, sf::Joystick::R));
    ImGui::Text("Eixo U: %.1f",
                sf::Joystick::getAxisPosition(0, sf::Joystick::U));
    ImGui::Text("Eixo V: %.1f",
                sf::Joystick::getAxisPosition(0, sf::Joystick::V));
    ImGui::End();
  }
  int gear = vehicle->getCurrentGear();
  float rpm = vehicle->getCurrentRPM();
  engineInstance->setParameterByName("RPM", rpm);
  ImGui::Begin("Car logs");
  ImGui::Text("Real Car Pos: X:%.2f Y:%.2f Z:%.2f", joltCarPos.GetX(),
              joltCarPos.GetY(), joltCarPos.GetZ());
  std::string gearText = "N";
  if (gear == -1)
    gearText = "R (Reverse)";
  else if (gear == 0)
    gearText = "N (Neutral)";
  else
    gearText = std::to_string(gear) + "-th gear";

  ImGui::Text("Actual gear: %s", gearText.c_str());
  ImGui::Text("Engine RPM: %.0f", rpm);

  static float torqueHistory[100] = {0.0f};

  for (int i = 0; i < 99; ++i) {
    torqueHistory[i] = torqueHistory[i + 1];
  }

  torqueHistory[99] = rpm;

  ImGui::PlotLines("RPM/Torque curve", torqueHistory, 100, 0, nullptr, 0.0f,
                   6000.0f, ImVec2(0, 80));
  ImGui::End();

  if (camera->mode == CameraMode::CHASE) {

    if (zdistance == 0)
      zdistance = 3;
    camera->offset.z = zdistance;
    camera->updateChase(carDummyPosition, carDummyForward, deltaTime);
  } else if (camera->mode == CameraMode::HOOD) {
    camera->updateHood(carDummyPosition, carDummyForward, carDummyUp);
  } else {
    if (game->window.hasFocus()) {
      sf::Vector2i center((int)game->window.getSize().x / 2,
                          (int)game->window.getSize().y / 2);
      sf::Vector2i mousePos = sf::Mouse::getPosition(game->window);
      sf::Vector2i delta = mousePos - center;
      camera->processMouseMovement((float)delta.x, -(float)delta.y);
      sf::Mouse::setPosition(center, game->window);
    }

    glm::vec3 moveInput(0.0f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
      moveInput.z += 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
      moveInput.z -= 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
      moveInput.x += 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
      moveInput.x -= 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
      moveInput.y += 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
      moveInput.y -= 1.0f;
    camera->updateFreeMove(deltaTime, moveInput);
  }

  float forward = 0.0f;
  float right = 0.0f;
  float brake = 0.0f;
  float handBrake = 0.0f;

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
      sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    forward = 1.0f;
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
           sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    forward = -1.0f;

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
      sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    right = 1.0f;
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
           sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    right = -1.0f;

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    handBrake = 1.0f;

  if (sf::Joystick::isConnected(0)) {

    float axisX = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
    if (std::abs(axisX) > 15.0f) {
      right = axisX / 100.0f;
    }

    float rtAxis = sf::Joystick::getAxisPosition(0, sf::Joystick::R);
    // Converte de [-100 a 100] para [0.0 a 1.0]
    float accelTrigger = (rtAxis + 100.0f) / 200.0f;

    float ltAxis = sf::Joystick::getAxisPosition(0, sf::Joystick::Z);

    float brakeTrigger = (ltAxis + 100.0f) / 200.0f;

    if (accelTrigger > 0.05f) {
      forward = accelTrigger;
    } else if (brakeTrigger > 0.05f) {
      forward = -brakeTrigger;
      brake = brakeTrigger;
    }

    if (sf::Joystick::isButtonPressed(0, 1)) {
      handBrake = 1.0f;
    }
  }

  // Envia os comandos para o veículo
  vehicle->setInput(forward, right, brake, handBrake);
}

void GameplayScene::render() {
  glClearColor(0.f, 0.46f, 0.91f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glPolygonMode(GL_FRONT_AND_BACK, wireframeMode ? GL_LINE : GL_FILL);

  shader->use();
  shader->setMat4("view", camera->getViewMatrix());
  shader->setMat4("projection", camera->getProjectionMatrix());

  shader->setVec3("light.position", globalLightPos);
  shader->setVec3("light.ambient", glm::vec3(0.4f, 0.4f, 0.4f));
  shader->setVec3("light.diffuse", glm::vec3(.6f, .6f, .6f));
  shader->setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
  shader->setVec3("viewPos", camera->position);
  shader->setFloat("time", (float)ticks);

  glm::mat4 model = glm::mat4(1.0f);
  // model = glm::translate(model, glm::vec3(2.f, 2.f, 0.f));
  // model = glm::scale(model, glm::vec3(.008f, .008f, .008f));
  // model = glm::rotate(model, glm::radians(ticks * .55f), glm::vec3(.0f, 1.f,
  // 0.f)); car->Draw(*shader, model);
  vehicle->draw(*shader);

  model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(-2.f, 2.f, 0.f));
  model = glm::scale(model, glm::vec3(.01f, .01f, .01f));
  model =
      glm::rotate(model, -glm::radians(ticks * .55f), glm::vec3(.0f, 1.f, 0.f));
  terrain->Draw(*shader, model);

  for (auto &entity : entities)
    entity.draw(*shader, physicsManager->body_interface);

  glDepthFunc(GL_LEQUAL);
  skyBoxShader->use();
  glm::mat4 view = glm::mat4(glm::mat3(camera->getViewMatrix()));
  skyBoxShader->setMat4("view", view);
  skyBoxShader->setMat4("projection", camera->getProjectionMatrix());
  skyBox->Draw(*skyBoxShader);
  glDepthFunc(GL_LESS);

  glUseProgram(0);
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_DEPTH_TEST);
}

void GameplayScene::refreshMouseState() {
  bool freeMode = (camera->mode == CameraMode::FREE);
  game->window.setMouseCursorVisible(!freeMode);
  game->window.setMouseCursorGrabbed(freeMode);

  if (freeMode) {
    sf::Vector2i center((int)game->window.getSize().x / 2,
                        (int)game->window.getSize().y / 2);
    sf::Mouse::setPosition(center, game->window);
  }
}
