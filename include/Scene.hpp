#ifndef SCENE_HPP
#define SCENE_HPP

#include <SFML/Window/Event.hpp>

class Game; 

class Scene
{
public:
    virtual ~Scene() = default;

    virtual void setup() = 0;
    virtual void handleEvent(sf::Event &event) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;

protected:
    Game *game = nullptr; 
    friend class Game;
};

#endif