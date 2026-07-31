#include <glad/glad.h>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include "./Mesh.h"
#include "./Shader.h"

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
    private:
    bool isActive= true;
};