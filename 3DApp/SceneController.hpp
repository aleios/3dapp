#pragma once
#ifndef SCENECONTROLLER_HPP
#define SCENECONTROLLER_HPP

#include <SFML/Graphics/RenderWindow.hpp>

#include "Scene.hpp"
#include "Modality.hpp"

namespace Scenes
{
    class SceneController
    {
    public:
        SceneController(sf::RenderWindow& inWindow)
            : window(inWindow)
        {
        }
        virtual ~SceneController()
        {
        }

        virtual void PushScene(const std::shared_ptr<Scene>& inScene, Modality inModality = Modality::Exclusive) = 0;
        virtual void PopScene() = 0;

        sf::RenderWindow& window;
    };
}

#endif