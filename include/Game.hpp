#include <glad/glad.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System/Clock.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include "./Mesh.hpp"
#include "./Shader.hpp"
#include "./Camera.hpp"
#include "./AssetsManager.hpp"
#include "./Model.hpp"

#include <glm/glm.hpp>

class Game
{
public:
    Game();
    void setup();
    void run();
    void update(float deltaTime);
    void handleEvents();
    void render();

    sf::ContextSettings contextSettings;
    sf::RenderWindow window;
    sf::Clock clock;
    float deltaTime, carSpeed;

    AssetsManager *assetsManager = new AssetsManager();
    Model *car;
    Mesh *triangle, *floor, *cube;
    Shader *shader, *lightSourceShader;
    Camera *camera;
    glm::vec3 carDummyPosition;
    glm::vec3 carDummyForward;
    glm::vec3 globalLightPos;

private:
    bool isActive = true;
};