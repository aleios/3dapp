#pragma once
#ifndef WORLD_HPP
#define WORLD_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

#include <SFML/Audio.hpp>

#include "Model.hpp"
#include "Shader.hpp"
#include "Camera.hpp"

#include "GBuffer.hpp"
#include "RenderTexture.hpp"
#include "PointLight.hpp"
#include "DirectionalLight.hpp"

#include "Quad.hpp"

enum class BufferTarget
    : unsigned int
{
    FINAL,
    DIFFUSE,
    POSITION,
    NORMAL,
    DEPTH,
    NUM_BUFFERS
};

class World
{
public:
    explicit World(sf::RenderWindow& inWindow);
    ~World();

    void OnEvent(const sf::Event& ev);
    void OnUpdate(const sf::Time& dt);
    void OnDraw();
private:
    Model pointLightMesh, spotLightMesh, directionalLightMesh;
    Shader pointLightShader, spotLightShader, directionalLightShader;

    RenderTexture aaTexture;
    RenderTexture hdrDownsampleTexture, hdrBlurTexture, hdrDownsample2Texture, hdrBlur2Texture;

    Shader hdrDownsample, hdrBlur, hdrTonemap, hdrBrightness;

    Model mdl, mdl2, mdl3, quadMesh;
    Shader pass1Shader, pass2Shader, pass2ShaderDirectional, quadShader, basicShader, emptyShader, flatShader, terrainShader, sphereShader;
    Shader fxaaShader, baseAAShader;
    Camera cam;

    sf::Texture mdlTex, terrainTexture, terrainWater;

    Quad fullscreenQuad;

    GBuffer gbuffer;

    std::vector<PointLight> pointLights;
    std::vector<DirectionalLight> directionalLights;

    bool mouseEnabled{ true };
    bool fshader{ false };
    bool terrainMove{ true };
    float mv{ 0.0f };
    float tv{ 0.0f };
    Vector3 spherePos;
    float exposure{ 1.0f };
    float exposureVar{ 1.0f };

    int lodLevel{ 0 };

    sf::RenderWindow& window;
    BufferTarget currentTarget{ BufferTarget::FINAL };

    sf::SoundBuffer sndExplodeBuffer;
    sf::Sound sndExplode;

    // Render passes
    void DoGeometryPass();
    void DoLightingPass(const PointLight& l);
    void DoDirectionalLightingPass(const DirectionalLight& l);

    // Stencil
    void DoStencilPass(const PointLight& l);
    void DoFinalPass();

    void DoAAPass();
    void DoHDRPass();

    // SFML Graphics
    sf::Font defaultFont;
    sf::Text helpText, targetText;
};

#endif