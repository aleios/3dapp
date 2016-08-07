#include "Game.hpp"
#include <assert.h>

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Window/Event.hpp>

#include "SceneGameplay.hpp"

#include "Settings.hpp"

void Game::Run()
{
    window.create(sf::VideoMode{ WINDOW_WIDTH, WINDOW_HEIGHT, 32 }, "3D test", 7U, sf::ContextSettings{ 24U, 8U, 0U, 4U, 2U });
    window.setFramerateLimit(120);
    window.setMouseCursorVisible(false);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    GLenum glewOk = glewInit();

    assert(glewOk == GLEW_OK);

    sceneManager = std::make_shared<Scenes::SceneManager>(window);

    std::shared_ptr<SceneGameplay> scene = std::make_shared<SceneGameplay>(sceneManager);
    sceneManager->PushScene(scene);

    gameClock.restart();
    while (window.isOpen())
    {
        sf::Time timeElapsed = gameClock.restart();
        std::ostringstream oss;
        oss << "FrameTime: " << timeElapsed.asMilliseconds();
        window.setTitle(oss.str());

        // Event handling
        sf::Event ev;
        std::vector<sf::Event> events;
        while (window.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
                window.close();

            events.push_back(ev);
        }

        sceneManager->OnUpdateInput(timeElapsed, events);

        sceneManager->OnUpdate(timeElapsed);

        if (window.isOpen())
            sceneManager->OnDraw(0.0f);
    }
}