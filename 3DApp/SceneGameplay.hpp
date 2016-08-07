#pragma once
#ifndef SCENEGAMEPLAY_HPP
#define SCENEGAMEPLAY_HPP

#include "Scene.hpp"
#include "World.hpp"

class SceneGameplay
    : public Scenes::Scene
{
public:
    SceneGameplay(const std::shared_ptr<Scenes::SceneController>& inController);
    void OnCreate();
    void OnUpdateInput(const sf::Time& dt, const std::vector<sf::Event>& events);
    void OnUpdate(const sf::Time& dt);
    void OnDraw();
    void OnDestroy();

private:
    std::unique_ptr<World> world;
};

#endif