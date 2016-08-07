#include "SceneManager.hpp"

using namespace Scenes;

SceneManager::SceneManager(sf::RenderWindow& inWindow)
    : SceneController(inWindow)
{

}

SceneManager::~SceneManager()
{
}

void SceneManager::PushScene(const std::shared_ptr<Scene>& inScene, Modality inModality)
{
    if (inScene == nullptr)
        ; // Exception

    // Add scene to the vector.
    scenes.push_back(ModalPair(inScene, inModality));

    inScene->OnCreate();

    // If the scene is exclusive then we clear up the sceneQueue so that only this scene can draw and update.
    if (inModality == Modality::Exclusive)
        sceneQueue.clear();

    sceneQueue.push_back(inScene.get());
}

void SceneManager::PopScene()
{
    auto& scene = scenes.back();
    scene.first->OnDestroy();

    if (scene.second == Modality::Exclusive)
        RebuildLists();
    else
        sceneQueue.pop_back();

    deleteQueue.push_back(scene);
    scenes.pop_back();
}

void SceneManager::Clear()
{
    sceneQueue.clear();
    scenes.clear();
}

void SceneManager::OnUpdateInput(const sf::Time& dt, const std::vector<sf::Event>& events)
{
    for (auto& scene : sceneQueue)
        scene->OnUpdateInput(dt, events);
}

void SceneManager::OnUpdate(const sf::Time& dt)
{
    for (auto& scene : sceneQueue)
        scene->OnUpdate(dt);
}

void SceneManager::OnDraw(float delta)
{
    for (auto& scene : sceneQueue)
        scene->OnDraw();
}

void SceneManager::RebuildLists()
{
    // Clear out the queues.
    sceneQueue.clear();

    // We have no scenes so no point in building nothing.
    if (scenes.empty())
        return;

    size_t index = scenes.size() - 1;
    while (index > 0)
    {
        if (scenes[index].second == Modality::Exclusive)
            break;
        --index;
    }

    while (index < scenes.size())
    {
        sceneQueue.push_back(scenes[index].first.get());
        ++index;
    }
}