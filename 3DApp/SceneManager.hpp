#pragma once
#ifndef SCENEMANAGER_HPP
#define SCENEMANAGER_HPP

#include "SceneController.hpp"

namespace Scenes
{
    class SceneManager
        : public SceneController
    {
    public:
        SceneManager(sf::RenderWindow& inWindow);
        SceneManager(const SceneManager&) = delete;
        ~SceneManager();

        SceneManager& operator=(const SceneManager&) = delete;

        void PushScene(const std::shared_ptr<Scene>& inScene, Modality inModality = Modality::Exclusive);
        void PopScene();
        void Clear();

        void OnUpdateInput(const sf::Time& dt, const std::vector<sf::Event>& events);
        void OnUpdate(const sf::Time& dt);
        void OnDraw(float delta);

        const std::size_t SceneCount() const
        {
            return scenes.size();
        }

    private:
        typedef std::pair<std::shared_ptr<Scene>, Modality> ModalPair;
        std::vector<ModalPair> scenes, deleteQueue;
        std::vector<Scene*> sceneQueue;

        void RebuildLists();
    };
}

#endif