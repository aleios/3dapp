#pragma once
#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "AEMath.hpp"

class Camera
{
public:
    Camera()
    {
        position = Vector3(0.0f, 0.0f, 0.0f);
        direction = Vector3(0.0f, 0.0f, -1.0f);
        up = Vector3(0.0f, 1.0f, 0.0f);
        rotation = Quaternion::Identity();
        viewMatrix = Matrix::CreateIdentity();

        pitch = roll = yaw = 0;

        nearClippingPlane = 0.1f;
        farClippingPlane = 400.0f;
        projectionMatrix = Matrix::Perspective(75.0f, 800.0f / 600.0f, nearClippingPlane, farClippingPlane);
    }

    void Update()
    {
        rotation = Quaternion::CreateFromYawPitchRoll(yaw, pitch, roll);
        Vector3 newUp = Quaternion::GetVectorTransform(rotation, up);
        newTarget = Quaternion::GetVectorTransform(rotation, direction) + position;
        viewMatrix = Matrix::CreateLookAt(position, newTarget, newUp);
    }

    void Move(const Vector3& pos)
    {
        //position += Quaternion::GetVectorTransform(rotation, pos);
        Quaternion prot = Quaternion::CreateFromYawPitchRoll(yaw, pitch, 0.0f);
        position += Quaternion::GetVectorTransform(prot, pos);
    }

    void Rotate(float yaw, float pitch, float roll)
    {
        this->yaw += yaw;
        if (this->yaw > 6.28f)
            this->yaw -= 6.28f;
        else if (this->yaw < 0)
            this->yaw += 6.28f;

        this->pitch += pitch;
        if (this->pitch > 1.1415f)
            this->pitch = 1.1415f;
        else if (this->pitch < -1.1415f)
            this->pitch = -1.1415f;

        this->roll += roll;
    }

    void SetPosition(const Vector3& pos)
    {
        position = pos;
    }

    void SetRotation(float yaw, float pitch, float roll)
    {
        this->pitch = pitch;
        this->yaw = yaw;
        this->roll = roll;
    }

    const Vector3& GetPosition() const { return position; }
    const Vector3& GetDirection() const { return newTarget; }
    const Vector3& GetUp() const { return newUp; }
    const Quaternion& GetRotation() const { return rotation; }

    const Matrix& GetViewMatrix()
    {
        return viewMatrix;
    }

    const Matrix& GetProjectionMatrix()
    {
        return projectionMatrix;
    }

    float GetNearClippingPlane() const { return nearClippingPlane; }
    float GetFarClippingPlane() const { return farClippingPlane; }
private:
    Vector3 position, direction, up;
    Vector3 newTarget, newUp;
    Quaternion rotation;
    float pitch, yaw, roll;

    float nearClippingPlane, farClippingPlane;

    Matrix projectionMatrix, viewMatrix;
};

#endif

/*
    What the camera is supposed to do?

    * Setup projection.
    * Move around the world.
    * Rotate the world view.

*/