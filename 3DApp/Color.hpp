#pragma once
#ifndef COLOR_HPP
#define COLOR_HPP

class Color
{
public:
    Color(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f)
        : r(r), g(g), b(b), a(a)
    {
    }

    float R() const
    {
        return r;
    }

    float G() const
    {
        return g;
    }

    float B() const
    {
        return b;
    }

    float A() const
    {
        return a;
    }

    float r, g, b, a;

    inline friend bool operator==(const Color& lhs, const Color& rhs);
    inline friend bool operator!=(const Color& lhs, const Color& rhs);
};

bool operator==(const Color& lhs, const Color& rhs)
{
    return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b && lhs.a == rhs.a;
}

bool operator!=(const Color& lhs, const Color& rhs)
{
    return !(lhs == rhs);
}

#endif