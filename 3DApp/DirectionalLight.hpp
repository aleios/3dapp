#pragma once
#ifndef DIRECTIONALLIGHT_HPP
#define DIRECTIONALLIGHT_HPP

#include "AEMath.hpp"

class DirectionalLight
{
public:
    void SetDirection(const Vector3& inDirection) { direction = inDirection; }
    const Vector3& GetDirection() const { return direction; }

    void SetColor(const Color& inColor) { lightColor = inColor; }
    const Color& GetColor() const { return lightColor; }

    // Color Intensity.
    void SetAmbientIntensity(float inIntensity) { ambientIntensity = inIntensity; }
    float GetAmbientIntensity() const { return ambientIntensity; }

    void SetDiffuseIntensity(float inIntensity) { diffuseIntensity = inIntensity; }
    float GetDiffuseIntensity() const { return diffuseIntensity; }

private:
    Vector3 direction;
    Color lightColor;
    float ambientIntensity{ 0.0f }, diffuseIntensity{ 0.0f };
};

#endif