#ifndef MENUSCENE_HPP
#define MENUSCENE_HPP

#include "./Scene.hpp"

class MenuScene : public Scene
{
public:
    void setup() override;
    void handleEvent(sf::Event &event) override;
    void update(float deltaTime) override;
    void render() override;
};

#endif