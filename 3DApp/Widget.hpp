#ifndef WIDGET_HPP
#define WIDGET_HPP

#include <vector>

#include <SFML/Window/Event.hpp>

class Widget
{
public:
    virtual void OnEvent(const std::vector<sf::Event>& events) = 0;
    virtual void OnDraw() = 0;

    bool IsVisible() const { return visible; }
    bool IsActive() const { return active; }
private:
    bool visible, active;
};

#endif