#ifndef GAMEPLAYSCENE_HPP
#define GAMEPLAYSCENE_HPP

#include "./Scene.hpp"
#include "./Mesh.hpp"
#include "./Shader.hpp"
#include "./Camera.hpp"
#include "./Model.hpp"
#include "./PhysicsManager.hpp"
#include "./Entity.hpp"
#include "./Skybox.hpp"
#include <glm/glm.hpp>
#include <vector>

class GameplayScene : public Scene
{
public:
    ~GameplayScene() override;

    void setup() override;
    void handleEvent(sf::Event &event) override;
    void update(float deltaTime) override;
    void render() override;
    void refreshMouseState();

private:
    std::vector<Entity> entities;
    Model *car, *terrain;
    Mesh *triangle;
    Shader *shader, *skyBoxShader;
    Camera *camera;
    PhysicsManager *physicsManager;
    SkyBox *skyBox;


    glm::vec3 carDummyPosition;
    glm::vec3 carDummyForward;
    glm::vec3 globalLightPos;
    float carSpeed = 0.0f;
    bool wireframeMode = false;
    int ticks = 0;
};

#endif