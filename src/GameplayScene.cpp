#include "../include/GameplayScene.hpp"
#include "../include/Game.hpp"
#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>
GameplayScene::~GameplayScene()
{
    delete physicsManager;
    delete shader;
    delete lightSourceShader;
    delete camera;
    delete triangle;
}

void GameplayScene::setup()
{

    game->window.pushGLStates();
    sf::Texture loadingTex;
    if (loadingTex.loadFromFile("../assets/Textures/loading.png"))
    {
        sf::Sprite loadingSprite(loadingTex);
        loadingSprite.setScale(sf::Vector2(.7f, .7f));
        loadingSprite.setPosition(0, -10);
        game->window.draw(loadingSprite);
    }
    game->window.popGLStates();

    game->window.display();
    physicsManager = new PhysicsManager();

    game->assetsManager->loadImage("../assets/Textures/dirt_ground.jpeg", "dirt_ground");
    game->assetsManager->loadImage("../assets/Textures/cobblestone.jpg", "cobble");
    game->assetsManager->loadImage("../assets/Textures/grass.jpg", "grass");
    game->assetsManager->loadImage("../assets/Textures/red_checker.png", "checker");

    car = new Model("../assets/Models/renault_5_alpine_cup_1976/scene.gltf", game->assetsManager);
    terrain = new Model("../assets/Models/lil_cow_-_harvest_moon_back_to_nature/scene.gltf", game->assetsManager);

    shader = new Shader("../assets/Shaders/default.vert", "../assets/Shaders/default.frag");
    lightSourceShader = new Shader("../assets/Shaders/lightsource.vert", "../assets/Shaders/lightsource.frag");

    globalLightPos = glm::vec3(2.f, 4.0f, 0.0f);
    camera = new Camera(1280.0f / 720.0f);

    carDummyPosition = glm::vec3(0.0f, 1.0f, 0.0f);
    carDummyForward = glm::vec3(0.0f, 0.0f, 1.0f);

    Model *floorModel = game->assetsManager->loadModel("../assets/Models/checkered_tile_floor/scene.gltf", "floor");
    JPH::BodyID floorId = physicsManager->createBox(
        glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3(100.0f, 1.0f, 100.0f),
        JPH::EMotionType::Static,
        Layers::NON_MOVING);
    Entity floorEntity;
    floorEntity.model = floorModel;
    floorEntity.hasPhysics = true;
    floorEntity.bodyId = floorId;
    floorEntity.transform.position = glm::vec3(0, 1.f, 0.);
    entities.push_back(floorEntity);

    Model *ballModel = game->assetsManager->loadModel("../assets/Models/sphere___low_poly/scene.gltf", "ball");
    Model *boxModel = game->assetsManager->loadModel("../assets/Models/Crate/Crate1.3ds", "box");

    float physicsRadius = 0.4f;
    float ballScale = physicsRadius / ballModel->getBoundingRadius();

    for (int i = -10; i < 10; i += 2)
    {
        JPH::BodyID sphereId = physicsManager->createSphere(
            glm::vec3(-((float)i) * 1.5, 20.0f, 0.f),
            physicsRadius, JPH::EMotionType::Dynamic, Layers::MOVING, 0.8f, 0.5f);

        Entity ballEntity;
        ballEntity.model = ballModel;
        ballEntity.hasPhysics = true;
        ballEntity.bodyId = sphereId;
        ballEntity.transform.scale = glm::vec3(ballScale * .9f);
        entities.push_back(ballEntity);

        for (int j = 0; j < 40; j += 2)
        {
            JPH::BodyID boxId = physicsManager->createBox(
                glm::vec3((float)j, 10.f + i, (float)i),
                glm::vec3(1.f, 1.f, 1.f), JPH::EMotionType::Dynamic, Layers::MOVING);

            Entity boxEntity;
            boxEntity.model = boxModel;
            boxEntity.hasPhysics = true;
            boxEntity.bodyId = boxId;
            entities.push_back(boxEntity);
        }
    }
}

void GameplayScene::handleEvent(sf::Event &event)
{
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::C)
    {
        camera->toggleMode();
        bool freeMode = (camera->mode == CameraMode::FREE);

        game->window.setMouseCursorVisible(!freeMode);
        game->window.setMouseCursorGrabbed(freeMode);

        if (freeMode)
        {
            sf::Vector2i center((int)game->window.getSize().x / 2, (int)game->window.getSize().y / 2);
            sf::Mouse::setPosition(center, game->window);
        }
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    {
        game->changeState(GameState::MENU);
    }
}

void GameplayScene::update(float deltaTime)
{
    ticks++;
    physicsManager->update(deltaTime);
    float zdistance = camera->offset.z;

    // camera->toggleMode();
    // bool freeMode = (camera->mode == CameraMode::FREE);

    ImGui::Begin("Zito-s-rally control panel");
    ImGui::Text("FPS: %.1f", 1.0f / deltaTime);
    ImGui::SliderFloat("Car speed", &carSpeed, -1.0f, 1.0f);
    ImGui::SliderFloat("Camera distance", &zdistance, 3.0f, 10.0f);
    ImGui::Text("Position: X:%.2f Y:%.2f Z:%.2f", carDummyPosition.x, carDummyPosition.y, carDummyPosition.z);
    if (ImGui::Button("Reset Position"))
        carDummyPosition = glm::vec3(0.0f, 1.0f, 0.0f);
    ImGui::Checkbox("Wireframe mode: ", &wireframeMode);
    ImGui::Text("Camera: %s (C to change, ESC to menu)",
                camera->mode == CameraMode::FREE ? "Free" : "Chasing car");
    ImGui::End();

    if (camera->mode == CameraMode::CHASE)
    {
        carDummyPosition += carDummyForward * (deltaTime * 0.001f);
        carDummyPosition.z += carSpeed;
        if (zdistance == 0)
            zdistance = 3;
        camera->offset.z = zdistance;
        camera->updateChase(carDummyPosition, carDummyForward, deltaTime);
    }
    else
    {
        if (game->window.hasFocus())
        {
            sf::Vector2i center((int)game->window.getSize().x / 2, (int)game->window.getSize().y / 2);
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

    globalLightPos.x -= sin(ticks / 100.f) / 2;
}

void GameplayScene::render()
{
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
    model = glm::translate(model, glm::vec3(2.f, 2.f, 0.f));
    // model = glm::scale(model, glm::vec3(.008f, .008f, .008f));
    model = glm::rotate(model, glm::radians(ticks * .55f), glm::vec3(.0f, 1.f, 0.f));
    car->Draw(*shader, model);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-2.f, 2.f, 0.f));
    model = glm::scale(model, glm::vec3(.01f, .01f, .01f));
    model = glm::rotate(model, -glm::radians(ticks * .55f), glm::vec3(.0f, 1.f, 0.f));
    terrain->Draw(*shader, model);

    for (auto &entity : entities)
        entity.draw(*shader, physicsManager->body_interface);

    glUseProgram(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_DEPTH_TEST);
}

void GameplayScene::refreshMouseState()
{
    bool freeMode = (camera->mode == CameraMode::FREE);
    game->window.setMouseCursorVisible(!freeMode);
    game->window.setMouseCursorGrabbed(freeMode);

    if (freeMode)
    {
        sf::Vector2i center((int)game->window.getSize().x / 2, (int)game->window.getSize().y / 2);
        sf::Mouse::setPosition(center, game->window);
    }
}