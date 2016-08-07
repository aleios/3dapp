#pragma once
#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

#include <memory>

#include "World.hpp"
#include "SceneManager.hpp"

class Game
{
public:
    void Run();

private:
    sf::RenderWindow window;
    sf::Clock gameClock;

    std::shared_ptr<Scenes::SceneManager> sceneManager;
};

#endif