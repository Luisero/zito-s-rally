#include "../../include/MenuScene.hpp"
#include "../../include/Game.hpp"
#include <imgui.h>
#include <SFML/Graphics.hpp>

sf::Sprite *backgroundSprite;
sf::Texture *backgroundTex;
void MenuScene::setup()
{
    game->window.setMouseCursorVisible(true);
    backgroundTex = new sf::Texture();
    if (backgroundTex->loadFromFile("./assets/Textures/loading.png"))
    {
        backgroundSprite = new sf::Sprite(*backgroundTex);
        backgroundSprite->setScale(sf::Vector2(.7f, .7f));
        backgroundSprite->setPosition(0, -10);
    }
}

void MenuScene::handleEvent(sf::Event &event)
{
}

void MenuScene::update(float deltaTime)
{
    game->window.setMouseCursorVisible(true);

    ImGui::SetNextWindowPos(ImVec2(540, 300));
    ImGui::Begin("Zito's Rally", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::Text("Wellcome!");

    if (ImGui::Button("Play", ImVec2(200, 50)))
    {
        game->changeState(GameState::PLAYING);
    }

    if (ImGui::Button("Exit", ImVec2(200, 50)))
    {
        game->isActive = false;
        game->window.close();
    }

    ImGui::End();
}

void MenuScene::render()
{
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    game->window.pushGLStates();
    game->window.draw(*backgroundSprite);
    game->window.popGLStates();
}