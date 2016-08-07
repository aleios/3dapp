#pragma once
#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <memory>

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

namespace Scenes
{
    class SceneController;
    class Scene
    {
    public:
        Scene(const std::shared_ptr<SceneController>& inController)
            : sceneController(inController)
        {
        }
        virtual void OnCreate() = 0;
        virtual void OnUpdateInput(const sf::Time& dt, const std::vector<sf::Event>& events) = 0;
        virtual void OnUpdate(const sf::Time& dt) = 0;
        virtual void OnDraw() = 0;
        virtual void OnDestroy() = 0;
    protected:
        std::shared_ptr<SceneController> sceneController;
    };
}

#endif