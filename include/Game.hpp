#ifndef GAME_HPP
#define GAME_HPP

#include "./AssetsManager.hpp"
#include "./Scene.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window.hpp>

#include <glad/glad.h>
#include <imgui-SFML.h>
#include <imgui.h>

enum class GameState { MENU, PLAYING };

class Game {
public:
  Game();
  void run();
  void changeState(GameState newState);

  sf::ContextSettings contextSettings;
  sf::RenderWindow window;
  sf::Clock clock;

  AssetsManager *assetsManager = new AssetsManager();

  GameState state = GameState::MENU;
  bool isActive = true;

private:
  void setupWindow();
  void handleEvents();
  void update(float deltaTime);
  void render();

  Scene *currentScene = nullptr;
  Scene *menuScene = nullptr;
  Scene *gameplayScene = nullptr;
};

#endif
