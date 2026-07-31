#include "../include/Game.hpp"
#include "../include/Mesh.hpp"
#include "../include/Shader.hpp"
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
    triangle = new Mesh();
    floor = new Mesh();
    shader = new Shader("../assets/Shaders/default.vert", "../assets/Shaders/default.frag");

    camera = new Camera(1280.0f / 720.0f);

    // mockup car position (just a test)
    carDummyPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    carDummyForward = glm::vec3(0.0f, 0.0f, 1.0f);

    triangle->vertices = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f};

    triangle->indices = {
        0, 1, 2};

    floor->vertices = {
       -1.f,0.f, 0.f,
       1.f,0.f,0.f,
       1.f,0.f, 3.f,
       -1.f, 0.f, 3.f
    };

    floor->indices = {
      0,1,2,
      0,2,3
    };

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

    ImGui::SFML::Update(window, sf::seconds(deltaTime));

    ImGui::Begin("Zito-s-rally control panel"); 
    
    
    ImGui::Text("FPS: %.1f", 1.0f / deltaTime); 
    
    ImGui::SliderFloat("Car speed", &carSpeed, 0.0f, 1.0f);
    
   
    ImGui::Text("Position: X:%.2f  Y:%.2f  Z:%.2f", carDummyPosition.x, carDummyPosition.y, carDummyPosition.z);
    
    if (ImGui::Button("Reset Position")) {
        carDummyPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    }
    ImGui::End();


    carDummyPosition += carDummyForward * (deltaTime * 0.001f);
    carDummyPosition.z += carSpeed;
    camera->updateChase(carDummyPosition, carDummyForward);
}
void Game::render()
{
    glClearColor(0.f, 0.46f, 0.91f, 1.0f); // Dark Teal Background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->use();

    shader->setMat4("view", camera->getViewMatrix());
    shader->setMat4("projection", camera->getProjectionMatrix());

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, carDummyPosition);
    model = glm::translate(model, glm::vec3(0.f, std::sin(ticks / 100.f), std::cos(ticks / 100.f)));

    float rotationAngle = glm::radians(ticks * 0.5f);

    model = glm::rotate(model, rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));

    shader->setMat4("model", model);

    triangle->draw();

    model = glm::mat4(1.0f);

    shader->setMat4("model", model);
    floor->draw();

    glUseProgram(0); 




    // 3. Desliga os Buffers e VAOs 
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
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

    ImGui::SFML::Shutdown();
}