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
#include <glm/glm.hpp>

class Game{
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

    Mesh* triangle, *floor;
    Shader* shader;
    Camera* camera;
    glm::vec3 carDummyPosition; 
    glm::vec3 carDummyForward;
    private:
    bool isActive= true;
};