#include "../include/Game.hpp"
#include "../include/Mesh.h"
#include "../include/Shader.h"
#include "iostream"
Game::Game() {};

void Game::setup()
{
    contextSettings.depthBits = 24;   // Request 24-bit depth buffer
    contextSettings.stencilBits = 8;  // Request 8-bit stencil buffer
    contextSettings.majorVersion = 3; // Request OpenGL 3.x
    contextSettings.minorVersion = 3; // Request OpenGL 3.3
    contextSettings.attributeFlags = sf::ContextSettings::Core;
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

    triangle = new Mesh();
    shader = new Shader("../assets/Shaders/default.vert", "../assets/Shaders/default.frag");

    triangle->vertices = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f};

    std::cout << triangle->vertices[0];

    triangle->setupMesh();
}
void Game::handleEvents()
{
    sf::Event event;

    while (window.pollEvent(event))
    {

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
}
void Game::render()
{
    glClearColor(0.f, 0.46f, 0.91f, 1.0f); // Dark Teal Background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->use();

    triangle->draw();
    window.display();
}

void Game::run()
{
    Game::setup();

    while (isActive)
    {

        Game::handleEvents();
        Game::render();
        Game::update(3.f);
    }
}