#pragma once
#ifndef SPOTLIGHT_HPP
#define SPOTLIGHT_HPP

#include "AEMath.hpp"
#include "Color.hpp"

class SpotLight
{
public:
    void SetPosition(const Vector3& inPosition) { position = inPosition; }
    void Move(const Vector3& inMoveVector) { position += inMoveVector; }
    const Vector3& GetPosition() const { return position; }

    void SetSpotAngle(float inAngle) { spotAngle = inAngle; }
    float GetSpotAngle() const { return spotAngle; }

    void SetDirection(const Vector3& inDirection) { direction = inDirection; }
    const Vector3& GetDirection() const { return direction; }

    void SetColor(const Color& inColor) { lightColor = inColor; }
    const Color& GetColor() const { return lightColor; }
private:
    float spotAngle;
    Vector3 position, direction;
    Color lightColor;
};

#endif