#include <glad/glad.h>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
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
    sf::Window  window;

    Mesh* triangle;
    Shader* shader;
    Camera* camera;
    glm::vec3 carDummyPosition; // Simulando a posição do carro
    glm::vec3 carDummyForward;
    private:
    bool isActive= true;
};