#include "../include/Game.hpp"
#include "../include/MenuScene.hpp"
#include "../include/GameplayScene.hpp"
#include <iostream>
#include <cstdlib>

Game::Game() {}

void Game::setupWindow()
{
    contextSettings.depthBits = 24;
    contextSettings.stencilBits = 8;
    contextSettings.majorVersion = 3;
    contextSettings.minorVersion = 3;
    contextSettings.attributeFlags = sf::ContextSettings::Default;
    window.create(
        sf::VideoMode({1280, 720}),
        "Zito's Rally",
        sf::Style::Fullscreen,
        contextSettings);
    window.setVerticalSyncEnabled(true);

    bool contextReady = false;
    for (int attempt = 0; attempt < 10 && !contextReady; ++attempt)
    {
        contextReady = window.setActive(true);
        if (!contextReady)
        {
            std::cerr << "Context not active yet, retrying... (" << attempt << ")" << std::endl;
            sf::sleep(sf::milliseconds(50));
        }
    }

    if (!contextReady)
    {
        std::cerr << "FATAL: Could not activate OpenGL context after retries" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    if (!gladLoadGL())
    {
        std::cerr << "FATAL: Failed to initialize GLAD" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    std::cout << glGetString(GL_VERSION) << "\n";
    glViewport(0, 0, 1280, 720);

    ImGui::SFML::Init(window);
}

void Game::changeState(GameState newState)
{
    state = newState;

    if (newState == GameState::MENU)
    {
        if (menuScene == nullptr)
        {
            menuScene = new MenuScene();
            menuScene->game = this;
            menuScene->setup();
        }
        currentScene = menuScene;
    }
    else // PLAYING
    {
        if (gameplayScene == nullptr)
        {
            gameplayScene = new GameplayScene();
            gameplayScene->game = this;
            gameplayScene->setup();
        }
        currentScene = gameplayScene;
        if (gameplayScene != nullptr)
        {
            ((GameplayScene *)gameplayScene)->refreshMouseState();
        }
    }
    clock.restart();
}

void Game::handleEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        ImGui::SFML::ProcessEvent(window, event);

        if (event.type == sf::Event::Closed)
        {
            isActive = false;
            window.close();
        }

        if (currentScene != nullptr)
            currentScene->handleEvent(event);
    }
}

void Game::update(float deltaTime)
{
    ImGui::SFML::Update(window, sf::seconds(deltaTime));

    if (currentScene != nullptr)
        currentScene->update(deltaTime);
}

void Game::render()
{
    if (currentScene != nullptr)
        currentScene->render();

    window.pushGLStates();
    ImGui::SFML::Render(window);
    window.popGLStates();

    window.display();
}

void Game::run()
{
    setupWindow();
    changeState(GameState::PLAYING);

    clock.restart();

    while (isActive)
    {
        sf::Time dt = clock.restart();
        float deltaTime = dt.asSeconds();
        handleEvents();
        update(deltaTime);
        render();
    }

    ImGui::SFML::Shutdown();
}