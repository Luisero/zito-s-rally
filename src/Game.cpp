#include "../include/Game.hpp"
#include "../include/Mesh.hpp"
#include "../include/Shader.hpp"
#include "../include/Texture.hpp"
#include <iostream>
#include <cmath>
Game::Game() {};

int ticks = 0;

void Game::setup()
{
    contextSettings.depthBits = 24;   // Request 24-bit depth buffer
    contextSettings.stencilBits = 8;  // Request 8-bit stencil buffer
    contextSettings.majorVersion = 3; // Request OpenGL 3.x
    contextSettings.minorVersion = 3; // Request OpenGL 3.3
    contextSettings.attributeFlags = sf::ContextSettings::Default;
    window.create(
        sf::VideoMode({1280, 720}),
        "My SFML Window",
        sf::Style::Fullscreen,
        contextSettings);
    window.setVerticalSyncEnabled(true);
    window.setActive(true);
    if (!gladLoadGL())
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
    }
    std::cout << glGetString(GL_VERSION) << "\n";
    glViewport(0, 0, 1280, 720);

    ImGui::SFML::Init(window);

    assetsManager->loadImage("../assets/Textures/dirt_ground.jpeg", "dirt_ground");
    assetsManager->loadImage("../assets/Textures/cobblestone.jpg", "cobble");
    assetsManager->loadImage("../assets/Textures/grass.jpg", "grass");
    assetsManager->loadImage("../assets/Textures/red_checker.png", "checker");

    Texture *ground_dir = new Texture(assetsManager->getImage("dirt_ground"));
    Texture *cobblestone = new Texture(assetsManager->getImage("cobble"));
    Texture *grass = new Texture(assetsManager->getImage("grass"));
    Texture *checker = new Texture(assetsManager->getImage("checker"));

    triangle = new Mesh(checker);
    floor = new Mesh(grass);
    shader = new Shader("../assets/Shaders/default.vert", "../assets/Shaders/default.frag");

    lightSourceShader = new Shader("../assets/Shaders/lightsource.vert", "../assets/Shaders/lightsource.frag");
    globalLightPos = glm::vec3(1.2f, 2.0f, 10.0f);
    camera = new Camera(1280.0f / 720.0f);

    // mockup car position (just a test)
    carDummyPosition = glm::vec3(0.0f, 1.0f, 0.0f);
    carDummyForward = glm::vec3(0.0f, 0.0f, 1.0f);

    triangle->vertices = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f};

    triangle->colors = {
        1.0f, 0.f, 0.f,
        0.f, 1.f, 0.f,
        0.f, 0.f, 1.f};

    triangle->indices = {0, 1, 2};

    triangle->UVs = {
        1.0f, 1.0f,
        1.0f, 0.f,
        0.f, 0.f};

    floor->vertices = {
        -30.f, 0.f, 30.f,
        30.f, 0.f, 30.f,
        30.f, 0.f, -3.f,
        -30.f, 0.f, -3.f};

    floor->indices = {
        0, 1, 2,
        0, 2, 3};

    floor->colors = {
        1.0f, 0.f, 0.f,
        0.f, 1.f, 0.f,
        0.f, 0.f, 1.f,
        1.0f, 0.f, 0.f,
        0.f, 1.f, 0.f,
        0.f, 0.f, 1.f};

    floor->UVs = {
        0.0f, 0.0f,   // Vértice 0: Canto inferior esquerdo
        30.0f, 0.0f,  // Vértice 1: Canto inferior direito (Repete 2x no X)
        30.0f, 30.0f, // Vértice 2: Canto superior direito (Repete 3x no Z)
        0.0f, 30.0f   // Vértice 3: Canto superior esquerdo
    };

    cube = new Mesh(cobblestone);

    cube->vertices = {
        -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
        0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f};

    cube->colors = {
        1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f,
        1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f,
        1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f,
        1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f,
        1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f,
        1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f};

    cube->indices = {
        0, 1, 2, 0, 2, 3,
        4, 5, 6, 4, 6, 7,
        8, 9, 10, 8, 10, 11,
        12, 13, 14, 12, 14, 15,
        16, 17, 18, 16, 18, 19,
        20, 21, 22, 20, 22, 23};

    cube->UVs = {
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f};

    cube->setupMesh();

    triangle->setupMesh();
    floor->setupMesh();
}
void Game::handleEvents()
{
    sf::Event event;

    while (window.pollEvent(event))
    {
        ImGui::SFML::ProcessEvent(window, event);

        // 1. Handle Window Close
        if (event.type == sf::Event::Closed)
        {
            isActive = false;
            window.close();
        }
    }
}
void Game::update(float deltaTime)
{
    ticks++;
    float zdistance = camera->offset.z;

    ImGui::SFML::Update(window, sf::seconds(deltaTime));

    ImGui::Begin("Zito-s-rally control panel");

    ImGui::Text("FPS: %.1f", 1.0f / deltaTime);

    ImGui::SliderFloat("Car speed", &carSpeed, 0.0f, 1.0f);
    ImGui::SliderFloat("Camera distance", &zdistance, 3.0f, 7.0f);

    ImGui::Text("Position: X:%.2f  Y:%.2f  Z:%.2f", carDummyPosition.x, carDummyPosition.y, carDummyPosition.z);

    if (ImGui::Button("Reset Position"))
    {
        carDummyPosition = glm::vec3(0.0f, 1.0f, 0.0f);
    }
    ImGui::End();

    carDummyPosition += carDummyForward * (deltaTime * 0.001f);
    carDummyPosition.z += carSpeed;
    if (zdistance == 0)
        zdistance = 3;
    camera->offset.z = zdistance;
    // carDummyForward.x +=.001f;
    camera->updateChase(carDummyPosition, carDummyForward, deltaTime);
}
void Game::render()
{
    glClearColor(0.f, 0.46f, 0.91f, 1.0f); // Dark Teal Background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    shader->use();

    shader->setMat4("view", camera->getViewMatrix());
    shader->setMat4("projection", camera->getProjectionMatrix());

    shader->setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
    shader->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    shader->setVec3("lightPos",globalLightPos);
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, carDummyPosition);
    model = glm::translate(model, glm::vec3(0.f, std::sin(ticks / 100.f) / 10.f, std::cos(ticks / 100.f)));

    float rotationAngle = glm::radians(ticks * 0.5f);

    model = glm::rotate(model, rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));

    shader->setMat4("model", model);

    triangle->draw(shader);

    model = glm::mat4(1.0f);

    shader->setMat4("model", model);
    floor->draw(shader);

    model = glm::mat4(1.0f);
    model = glm::translate(model, globalLightPos);
    model = glm::scale(model, glm::vec3(0.2f));
    lightSourceShader->use();
    lightSourceShader->setMat4("model", model);

    lightSourceShader->setMat4("view", camera->getViewMatrix());
    lightSourceShader->setMat4("projection", camera->getProjectionMatrix());

    cube->draw(lightSourceShader);

    glUseProgram(0);

    // 3. Desliga os Buffers e VAOs
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_DEPTH_TEST);

    window.pushGLStates();

    ImGui::SFML::Render(window);

    window.popGLStates();

    window.display();
}

void Game::run()
{
    Game::setup();

    while (isActive)
    {
        sf::Time dt = clock.restart();
        float deltaTime = dt.asSeconds();
        Game::handleEvents();
        Game::update(deltaTime);
        Game::render();
    }
    glDeleteProgram(shader->ID);

    ImGui::SFML::Shutdown();
}