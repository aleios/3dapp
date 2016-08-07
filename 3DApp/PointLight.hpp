#pragma once
#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "AEMath.hpp"
#include "Color.hpp"
#include "BoundingSphere.hpp"

class PointLight
{
public:
    void SetPosition(const Vector3& inPosition) { position = inPosition; collider.center = position; }
    void Move(const Vector3& inMoveVector) { position += inMoveVector; }
    const Vector3& GetPosition() const { return position; }

    void SetScale(const Vector3& inScale) { scale = inScale; collider.radius = scale.x; }
    const Vector3& GetScale() const { return scale; }

    void SetColor(const Color& inColor) { lightColor = inColor; }
    const Color& GetColor() const { return lightColor; }

    // Color Intensity.
    void SetAmbientIntensity(float inIntensity) { ambientIntensity = inIntensity; }
    float GetAmbientIntensity() const { return ambientIntensity; }
    
    void SetDiffuseIntensity(float inIntensity) { diffuseIntensity = inIntensity; }
    float GetDiffuseIntensity() const { return diffuseIntensity; }

    // Attenuation.
    void SetAttenuationConstant(float inAttenuation) { attenuationConstant = inAttenuation; }
    float GetAttenuationConstant() const { return attenuationConstant; }

    void SetAttenuationLinear(float inAttenuation) { attenuationLinear = inAttenuation; }
    float GetAttenuationLinear() const { return attenuationLinear; }

    void SetAttenuationExponential(float inAttenuation) { attenuationExponential = inAttenuation; }
    float GetAttenuationExponential() const { return attenuationExponential; }

    Matrix GetMatrix() const
    {
        return Matrix::CreateScale(scale) * Matrix::CreateTranslation(position);
    }

    const BoundingSphere& GetCollider()
    {
        return collider;
    }
private:
    Vector3 position, scale;
    Color lightColor;
    BoundingSphere collider;

    float ambientIntensity{ 0.0f }, diffuseIntensity{ 0.0f };
    float attenuationConstant{ 0.0f }, attenuationLinear{ 0.0f }, attenuationExponential{ 0.0f };
};

#endif