#include "SceneGameplay.hpp"
#include "SceneController.hpp"

SceneGameplay::SceneGameplay(const std::shared_ptr<Scenes::SceneController>& inController)
: Scene(inController)
{

}

void SceneGameplay::OnCreate()
{
    world = std::make_unique<World>(sceneController->window);
}

void SceneGameplay::OnUpdateInput(const sf::Time& dt, const std::vector<sf::Event>& events)
{
    for (auto& ev : events)
        world->OnEvent(ev);
}

void SceneGameplay::OnUpdate(const sf::Time& dt)
{
    world->OnUpdate(dt);
}

void SceneGameplay::OnDraw()
{
    world->OnDraw();
}

void SceneGameplay::OnDestroy()
{

}