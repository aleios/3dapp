#include "World.hpp"
#include <algorithm>

#include "Settings.hpp"

#define USE_FXAA 0

float float_rand(float a, float b)
{
    return ((b - a)*((float)rand() / RAND_MAX)) + a;
}

const static float maxCol = 5.0f;

World::World(sf::RenderWindow& inWindow)
: window(inWindow), gbuffer(WINDOW_WIDTH, WINDOW_HEIGHT), hdrDownsampleTexture(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, true), hdrBlurTexture(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, true), hdrDownsample2Texture(WINDOW_WIDTH / 4, WINDOW_HEIGHT / 4, true), hdrBlur2Texture(WINDOW_WIDTH / 4, WINDOW_HEIGHT / 4, true), fullscreenQuad(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT), aaTexture(WINDOW_WIDTH, WINDOW_HEIGHT)
{
    // OpenGL State
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);
    glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
    glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

    // Seed the PRNG.
    srand(static_cast<unsigned int>(time(0)));

    // Load light meshes.
    pointLightMesh.LoadFromFile("Models/pointlightMesh.obj");
    spotLightMesh.LoadFromFile("Models/spotlightMesh.obj");
    directionalLightMesh.LoadFromFile("Models/directionalMesh.obj");

    // Lighting Shaders.
    pointLightShader.LoadFromFile("Shaders/lightShader.vert", "Shaders/pointlightShader.frag");
    spotLightShader.LoadFromFile("Shaders/lightShader.vert", "Shaders/spotlightShader.frag");
    directionalLightShader.LoadFromFile("Shaders/lightShader.vert", "Shaders/directionalShader.frag");

    // HDR Shaders.
    hdrDownsample.LoadFromFile("Shaders/fsquad.vert", "Shaders/hdrDownsample.frag");
    hdrTonemap.LoadFromFile("Shaders/fsquad.vert", "Shaders/hdrTonemap.frag");
    hdrBlur.LoadFromFile("Shaders/fsquad.vert", "Shaders/hdrBlur.frag");
    hdrBrightness.LoadFromFile("Shaders/fsquad.vert", "Shaders/hdrBrightness.frag");

    // Load Models.
    mdl2.LoadFromFile("Models/sphere.obj");
    mdl.LoadFromFile("Models/floor.obj");
    mdl3.LoadFromFile("Models/sphere.obj");
    quadMesh.LoadFromFile("Models/quad.obj");

    // Load Shaders
    basicShader.LoadFromFile("Shaders/basic.vert", "Shaders/basic.frag");
    pass1Shader.LoadFromFile("Shaders/drpass1.vert", "Shaders/drpass1.frag");
    pass2Shader.LoadFromFile("Shaders/drpass2.vert", "Shaders/drpass2.frag");
    pass2ShaderDirectional.LoadFromFile("Shaders/empty.vert", "Shaders/drpass2directional.frag");
    quadShader.LoadFromFile("Shaders/fsquad.vert", "Shaders/hdrTonemap.frag");
    terrainShader.LoadFromFile("Shaders/terrainshader.vert", "Shaders/terrainshader.frag");
    flatShader.LoadFromFile("Shaders/basic.vert", "Shaders/flatshade.frag");
    emptyShader.LoadFromFile("Shaders/empty.vert", "Shaders/empty.frag");
    sphereShader.LoadFromFile("Shaders/sphere.vert", "Shaders/sphere.frag");

    // Anti-Aliasing Shaders
#if USE_FXAA == 1
    fxaaShader.LoadFromFile("Shaders/fxaa.vert", "Shaders/fxaa.frag");
#endif
    baseAAShader.LoadFromFile("Shaders/baseaa.vert", "Shaders/baseaa.frag");

    // Load textures.
    mdlTex.loadFromFile("Textures/test.png");
    terrainTexture.loadFromFile("Textures/terraintex.png");
    terrainWater.loadFromFile("Textures/terrainwater.png");

    // Main world directional light.
    DirectionalLight worldLight;
    worldLight.SetDirection(Vector3(-1.0f, 0.0f, 0.0f));
    worldLight.SetColor(Color(0.8f, 0.8f, 0.8f));
    worldLight.SetDiffuseIntensity(0.9f);
    worldLight.SetAmbientIntensity(0.4f);
    directionalLights.push_back(worldLight);

    // Lights
    const unsigned int numLights = 40;
    const float lightRadius = 2.0f;
    PointLight l;
    l.SetPosition(Vector3(10.0f, 2.0f, 10.0f));
    l.SetScale(Vector3(3.0f, 3.0f, 3.0f));
    l.SetColor(Color(0.0f, 0.0f, 1.4f, 1.0f));
    l.SetAttenuationConstant(0.4f);
    l.SetDiffuseIntensity(0.4f);
    l.SetAmbientIntensity(0.2f);
    pointLights.push_back(l);

    const int maxXZ = 15;
    const int minXZ = -15;
    const int maxY = 5;
    const float minRad = 0.3f;
    const float maxRad = 2.0f;
    for (int i = 0; i < numLights; i++)
    {
        PointLight l;
        
        // Generate position.
        int x = (rand() % (maxXZ - minXZ)) + minXZ;
        int y = (rand() % maxY);
        int z = (rand() % (maxXZ - minXZ)) + minXZ;
        l.SetPosition(Vector3(x, y, z));

        // Generate Scale
        float frand = ((float)rand() / (float)RAND_MAX);
        float fdiff = maxRad - minRad;

        //l.SetScale(Vector3(scale, scale, scale));

        // Generate Color
        float r = float_rand(0.0f, maxCol);
        float g = float_rand(0.0f, maxCol);
        float b = float_rand(0.0f, maxCol);
        l.SetColor(Color(r, g, b, 1.0f));

        float ambient = ((float)rand() / (float)RAND_MAX);
        float diff = ((float)rand() / (float)RAND_MAX);

        l.SetAmbientIntensity(ambient);
        l.SetDiffuseIntensity(diff);

        float catt = ((float)rand() / (float)RAND_MAX);
        float latt = ((float)rand() / (float)RAND_MAX);
        float eatt = ((float)rand() / (float)RAND_MAX);

        float mc = fmax(fmax((float)r, (float)g), (float)b);
        float ret = (-latt + sqrtf(latt * latt - 4 * eatt * (eatt - 256 * mc * diff))) / 2 * eatt;

        l.SetScale(Vector3(ret, ret, ret));

        l.SetAttenuationConstant(catt);
        l.SetAttenuationLinear(latt);
        l.SetAttenuationExponential(eatt);

        pointLights.push_back(l);
    }

    // Set the initial position of the sphere
    spherePos = Vector3(10.0f, 3.0f, 10.0f);

    // Setup the help text.
    defaultFont.loadFromFile("arial.ttf");
    helpText.setFont(defaultFont);
    helpText.setString("Controls:\n" \
                       "Mouse: Camera direction.\n" \
                       "WASD: Camera movement.\n" \
                       "I: Randomize light attenuations.\n" \
                       "K: Randomize light color and intensity.\n" \
                       "Insert: Disable mouse.\n" \
                       "Delete: Switch shown render target.\n" \
                       "Home: Disable/Enable terrain movement.\n"
                       );
    helpText.setCharacterSize(14);
    helpText.setColor(sf::Color(255, 0, 255, 255));

    targetText.setFont(defaultFont);
    targetText.setCharacterSize(14);
    targetText.setPosition(sf::Vector2f(0.0, window.getSize().y - 25));
    targetText.setString("Current Buffer: Final Output");
    targetText.setColor(sf::Color(255, 0, 255, 255));

    // Sounds
    sndExplodeBuffer.loadFromFile("explode.wav");
    sndExplode.setBuffer(sndExplodeBuffer);
    sndExplode.setAttenuation(10.0f);
    sndExplode.setMinDistance(60.0f);
    sndExplode.setPosition(sf::Vector3f(0.0f, 1.0f, 0.0f));
}

World::~World()
{

}

void World::OnEvent(const sf::Event& ev)
{
    if (ev.type == sf::Event::KeyReleased)
    {
        if (ev.key.code == sf::Keyboard::Insert)
        {
            mouseEnabled = !mouseEnabled;
            window.setMouseCursorVisible(!mouseEnabled);
        }
        else if (ev.key.code == sf::Keyboard::Home)
            terrainMove = !terrainMove;
        else if (ev.key.code == sf::Keyboard::Escape)
            window.close();
        else if (ev.key.code == sf::Keyboard::Delete)
        {
            // Change the current target.
            unsigned int target = (unsigned int)currentTarget + 1;
            if (target >= (unsigned int)BufferTarget::NUM_BUFFERS)
                target = 0;
            currentTarget = (BufferTarget)target;

            // Update the text.
            std::string bufferText = "";
            switch (currentTarget)
            {
            default:
            case BufferTarget::FINAL:
                bufferText = "Final output";
                break;
            case BufferTarget::DIFFUSE:
                bufferText = "Diffuse";
                break;
            case BufferTarget::POSITION:
                bufferText = "Position";
                break;
            case BufferTarget::NORMAL:
                bufferText = "Normal";
                break;
            case BufferTarget::DEPTH:
                bufferText = "Depth";
                break;
            }
            targetText.setString("Current Buffer: " + bufferText + ".");
        }
        else if (ev.key.code == sf::Keyboard::T)
        {
            lodLevel += 1;
            std::cout << "Blur Passes: " << lodLevel << "\n";
        }
        else if (ev.key.code == sf::Keyboard::G)
        {
            lodLevel -= 1;
            std::cout << "Blur Passes: " << lodLevel << "\n";
        }
    }
}

void World::OnUpdate(const sf::Time& dt)
{
    // Update the camera.
    cam.Update();

    // Move the camera.
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        cam.Move(Vector3(0.0f, 0.0f, -5.0f * dt.asSeconds()));
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        cam.Move(Vector3(0.0f, 0.0f, 5.0f * dt.asSeconds()));
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        cam.Move(Vector3(-5.0f * dt.asSeconds()));
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        cam.Move(Vector3(5.0f * dt.asSeconds()));
    }

    // Rotate camera via mouse.
    sf::Vector2i mpos = sf::Mouse::getPosition(window);
    if(mouseEnabled) sf::Mouse::setPosition(sf::Vector2i(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), window);
    sf::Vector2i deltaPos = sf::Mouse::getPosition(window) - mpos;
    cam.Rotate(deltaPos.x * 1.5f * dt.asSeconds(), deltaPos.y * 1.5f * dt.asSeconds(), 0.0f);

    // Sound Spatialization
    //std::cout << "Pos: " << cam.GetPosition() << "\n";
    sf::Listener::setPosition(cam.GetPosition().ToSFMLVec());
    sf::Listener::setDirection(cam.GetDirection().ToSFMLVec());
    sf::Listener::setUpVector(cam.GetUp().ToSFMLVec());

   // if (sndExplode.getStatus() == sf::Sound::Stopped)
        //sndExplode.play();

    // Move light up and down.
    Vector3 lcpos = pointLights[0].GetPosition();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        exposure -= 1.0f * dt.asSeconds();
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        exposure += 1.0f * dt.asSeconds();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        const Color& col = pointLights[0].GetColor();
        pointLights[0].SetColor(Color(col.R(), col.G(), col.B() + 1.0f * dt.asSeconds()));
        std::cout << col.B() << "\n";
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        const Color& col = pointLights[0].GetColor();
        pointLights[0].SetColor(Color(col.R(), col.G(), col.B() - 1.0f * dt.asSeconds()));
        std::cout << col.B() << "\n";
    }

    // Randomize Attenuations.
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
    {
        for (auto& l : pointLights)
        {
            float catt = ((float)rand() / (float)RAND_MAX);
            float latt = ((float)rand() / (float)RAND_MAX);
            float eatt = ((float)rand() / (float)RAND_MAX);

            l.SetAttenuationConstant(catt);
            l.SetAttenuationLinear(latt);
            l.SetAttenuationExponential(eatt);

            const Color& c = l.GetColor();
            float mc = fmax(fmax((float)c.R(), (float)c.G()), (float)c.B());
            float ret = (-latt + sqrtf(latt * latt - 4 * eatt * (eatt - 256 * mc * l.GetDiffuseIntensity()))) / 2 * eatt;

            l.SetScale(Vector3(ret, ret, ret));
        }
    }

    // Randomize intensity.
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
    {
        for (auto& l : pointLights)
        {
            float ambient = ((float)rand() / (float)RAND_MAX);
            float diff = ((float)rand() / (float)RAND_MAX);

            l.SetAmbientIntensity(ambient);
            l.SetDiffuseIntensity(diff);

            // Generate Color
            float r = float_rand(0.0f, maxCol);
            float g = float_rand(0.0f, maxCol);
            float b = float_rand(0.0f, maxCol);
            l.SetColor(Color(r, g, b, 1.0f));
        }
    }

    // Uniforms
    mv += 0.2f * dt.asSeconds();
    pointLights[0].SetPosition(Matrix::GetVectorTransform(Matrix::CreateYRotation(mv), spherePos));

    if (terrainMove)
        tv += 0.1f * dt.asSeconds();

    /*if (exposure >= 10.0f)
        exposureVar = -1.0f;
    else if (exposureVar <= 0.0f)
        exposureVar = 1.0f;

    exposure += exposureVar * dt.asSeconds();*/
}

void World::OnDraw()
{

    gbuffer.StartBuffer();
    // Render geometry data to the GBuffer MRT.
    DoGeometryPass();

    // Enable the stencil test.
    glEnable(GL_STENCIL_TEST);

    // Perform stencil and draw lights for each single light.
    for (auto& l : pointLights)
    {
        // Put Stencil data into the Final FBO but disable drawing.
        DoStencilPass(l);

        // Put lighting data into the Final FBO.
        DoLightingPass(l);
    }

    // Disable the stencil test.
    glDisable(GL_STENCIL_TEST);

    // Draw full screen light when debug is not enabled.
    /*for (auto& l : directionalLights)
        DoDirectionalLightingPass(l);*/

    // Switch back to the main buffer.
    gbuffer.EndBuffer();

    // Anti-Aliasing
    DoAAPass();

    // HDR Rendering
    DoHDRPass();

    // Render final output.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    DoFinalPass();

    // SFML Rendering
    window.pushGLStates();
    window.resetGLStates();
    window.draw(helpText);
    window.draw(targetText);
    glPopAttrib();
    window.popGLStates();

    window.display();
}

// ===================
// Geometry Render
// ===================
void World::DoGeometryPass()
{
    gbuffer.StartGeomPass();
    // Enable depth write and test.
    glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    Shader* geomShader = &pass1Shader;
    Shader::Bind(geomShader);

    Matrix pmat = cam.GetProjectionMatrix();
    Matrix vmat = cam.GetViewMatrix();
    geomShader->SetParameter("projectionMatrix", &pmat[0][0]);
    geomShader->SetParameter("viewMatrix", &vmat[0][0]);

    // Queen object
    geomShader->SetParameter("specularIntensity", 1.2f);
    geomShader->SetParameter("specularPower", 200.0f);
    geomShader->SetParameter("farClipDistance", cam.GetFarClippingPlane());

    sf::Texture::bind(&mdlTex);
    //glBindTexture(GL_TEXTURE_2D, mdlTex.)
    Matrix queenModelMat = Matrix::CreateTranslation(0.0f, 1.0f, 0.0f);
    geomShader->SetParameter("modelMatrix", &queenModelMat[0][0]);
    mdl2.Draw();
    sf::Texture::bind(0);

    // Sphere
    Shader::Bind(&sphereShader);
    Matrix mmat = Matrix::CreateTranslation(spherePos);
    sphereShader.SetParameter("projectionMatrix", &pmat[0][0]);
    sphereShader.SetParameter("viewMatrix", &vmat[0][0]);
    sphereShader.SetParameter("modelMatrix", &mmat[0][0]);
    sphereShader.SetParameter("rotateValue", mv);
    sphereShader.SetParameter("specularIntensity", 1000.0f);
    sphereShader.SetParameter("specularPower", 100.0f);
    //sphereShader.SetParameter("farClipDistance", cam.GetFarClippingPlane());
    mdl3.Draw();

    // Terrain
    // We can use multiple different shaders in a pass
    // The fragment shader must output the required values though.
    Shader::Bind(&terrainShader);

    glActiveTexture(GL_TEXTURE0);
    sf::Texture::bind(&terrainWater);

    glActiveTexture(GL_TEXTURE1);
    sf::Texture::bind(&terrainTexture);

    terrainShader.SetParameter("projectionMatrix", &pmat[0][0]);
    terrainShader.SetParameter("viewMatrix", &vmat[0][0]);
    geomShader->SetParameter("specularIntensity", 0.0f);
    geomShader->SetParameter("specularPower", 0.0f);
    geomShader->SetParameter("farClipDistance", cam.GetFarClippingPlane());

    terrainShader.SetParameter("watertex", 0);
    terrainShader.SetParameter("groundtex", 1);

    Matrix terrMat = Matrix::CreateIdentity();
    terrainShader.SetParameter("modelMatrix", &terrMat[0][0]);
    terrainShader.SetParameter("time", tv);

    mdl.Draw();

    // Disable depth write
    glDepthMask(GL_FALSE);
}

// ===================
// Point lights
// ===================
void World::DoLightingPass(const PointLight& l)
{
    gbuffer.StartLightingPass();

    // Disable depth test.
    glDisable(GL_DEPTH_TEST);

    // Setup for lighting pass.
    glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

    glEnable(GL_BLEND);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    // Bind pass 2 shader.
    Shader* lpassShader = &pointLightShader;
    Shader::Bind(lpassShader);

    // Activate gbuffer textures.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gbuffer.GetColorID());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gbuffer.GetNormalID());
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gbuffer.GetPositionID());

    // Matrices
    Matrix pmat = cam.GetProjectionMatrix();
    Matrix vmat = cam.GetViewMatrix();

    lpassShader->SetParameter("projectionMatrix", &pmat[0][0]);
    lpassShader->SetParameter("viewMatrix", &vmat[0][0]);
    lpassShader->SetParameter("screenSize", WINDOW_WIDTH, WINDOW_HEIGHT);

    // Set the eye position to the camera position.
    Vector3 cpos = cam.GetPosition();
    lpassShader->SetParameter("eyePosition", cpos.x, cpos.y, cpos.z);
    lpassShader->SetParameter("farClipDistance", cam.GetFarClippingPlane());

    // Texture locations
    lpassShader->SetParameter("difftex", 0);
    lpassShader->SetParameter("normtex", 1);
    lpassShader->SetParameter("postex", 2);

    // Draw point lights.
    Matrix lmat = l.GetMatrix();
    lpassShader->SetParameter("modelMatrix", &lmat[0][0]);

    const Vector3& pos = l.GetPosition();
    lpassShader->SetParameter("lightPos", pos.x, pos.y, pos.z);

    // Itensity.
    lpassShader->SetParameter("ambientIntensity", l.GetAmbientIntensity());
    lpassShader->SetParameter("diffuseIntensity", l.GetDiffuseIntensity());

    // Attenuation.
    lpassShader->SetParameter("attenuationConstant", l.GetAttenuationConstant());
    lpassShader->SetParameter("attenuationLinear", l.GetAttenuationLinear());
    lpassShader->SetParameter("attenuationExponential", l.GetAttenuationExponential());

    Color diffuseColor = l.GetColor();
    lpassShader->SetParameter("lightColor", diffuseColor.R(), diffuseColor.G(), diffuseColor.B());

    pointLightMesh.Draw();

    glCullFace(GL_BACK);
    glDisable(GL_BLEND);
}

// ===================
// Stencil pass
// ===================
void World::DoStencilPass(const PointLight& l)
{
    gbuffer.StartStencilPass();
    glClear(GL_STENCIL_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glStencilFunc(GL_ALWAYS, 0, 0);

    // Bind pass 2 shader.
    Shader* lpassShader = &emptyShader;
    Shader::Bind(lpassShader);

    // Matrices
    Matrix pmat = cam.GetProjectionMatrix();
    Matrix vmat = cam.GetViewMatrix();

    lpassShader->SetParameter("projectionMatrix", &pmat[0][0]);
    lpassShader->SetParameter("viewMatrix", &vmat[0][0]);

    // Draw point light.
    Matrix lmat = l.GetMatrix();
    lpassShader->SetParameter("modelMatrix", &lmat[0][0]);

    mdl3.Draw();
}

// ===================
// Directional lights
// ===================
void World::DoDirectionalLightingPass(const DirectionalLight& l)
{
    Shader* lpassShader = &directionalLightShader;
    Shader::Bind(lpassShader);

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    // Activate gbuffer textures.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gbuffer.GetColorID());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gbuffer.GetNormalID());
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gbuffer.GetPositionID());

    // Matrices
    Matrix om = cam.GetProjectionMatrix();
    lpassShader->SetParameter("projectionMatrix", &om[0][0]);

    Matrix vdmat = Matrix::CreateIdentity();
    lpassShader->SetParameter("viewMatrix", &vdmat[0][0]);
    lpassShader->SetParameter("screenSize", WINDOW_WIDTH, WINDOW_HEIGHT);

    // Set the eye position to the camera position.
    Vector3 cpos = cam.GetPosition();
    lpassShader->SetParameter("eyePosition", cpos.x, cpos.y, cpos.z);

    // Texture locations
    lpassShader->SetParameter("difftex", 0);
    lpassShader->SetParameter("normtex", 1);
    lpassShader->SetParameter("postex", 2);

    Matrix lmat = Matrix::CreateIdentity();
    lpassShader->SetParameter("modelMatrix", &lmat[0][0]);

    const Vector3& dir = l.GetDirection();
    lpassShader->SetParameter("lightDir", dir.x, dir.y, dir.z);

    const Color& color = l.GetColor();
    lpassShader->SetParameter("lightColor", color.r, color.g, color.b);

    lpassShader->SetParameter("ambientIntensity", l.GetAmbientIntensity());
    lpassShader->SetParameter("diffuseIntensity", l.GetDiffuseIntensity());

    directionalLightMesh.Draw();

    glDisable(GL_BLEND);
}

// ===================
// Anti-Aliasing Pass
// ===================
void World::DoAAPass()
{
    aaTexture.Bind();

#if USE_FXAA == 0
    Shader::Bind(&baseAAShader);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gbuffer.GetFinalID());

    baseAAShader.SetParameter("tex", 0);

#else
    Shader::Bind(&fxaaShader);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gbuffer.GetFinalID());

    fxaaShader.SetParameter("tex", 0);

#endif

    // Render output
    fullscreenQuad.Render();
    aaTexture.Unbind();
}

// ===================
// HDR Pass
// ===================
void World::DoHDRPass()
{
    Matrix pmat = Matrix::OrthoProjection(WINDOW_WIDTH, 0.0f, WINDOW_HEIGHT, 0.0f, -1, 1);
    Matrix vmat = Matrix::CreateIdentity();
    
    // Downsample to 1/2

    hdrBlurTexture.Bind();

    Shader::Bind(&hdrBrightness);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, aaTexture.GetTexID());

    hdrBrightness.SetParameter("projectionMatrix", &pmat[0][0]);
    hdrBrightness.SetParameter("viewMatrix", &vmat[0][0]);
    hdrBrightness.SetParameter("modelMatrix", &vmat[0][0]);
    hdrBrightness.SetParameter("tex", 0);

    fullscreenQuad.Render();

    hdrBlurTexture.Unbind();

    int numBlurPasses = lodLevel;
    for (int i = 0; i < numBlurPasses; i++)
    {
        // Blur #1 Vertical
        hdrDownsampleTexture.Bind();

        Shader::Bind(&hdrBlur);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, hdrBlurTexture.GetTexID());

        hdrBlur.SetParameter("projectionMatrix", &pmat[0][0]);
        hdrBlur.SetParameter("viewMatrix", &vmat[0][0]);
        hdrBlur.SetParameter("modelMatrix", &vmat[0][0]);
        hdrBlur.SetParameter("tex", 0);
        hdrBlur.SetParameter("offset", 0.0f, 1.0f / (WINDOW_HEIGHT / 2.0f));
        hdrBlur.SetParameter("lodlevel", 0);

        fullscreenQuad.Render();

        hdrDownsampleTexture.Unbind();

        // Blur #1 Horizontal
        hdrBlurTexture.Bind();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, hdrDownsampleTexture.GetTexID());
        hdrBlur.SetParameter("tex", 0);
        hdrBlur.SetParameter("offset", 1.0f / (WINDOW_WIDTH / 2.0f), 0.0f);

        fullscreenQuad.Render();

        hdrBlurTexture.Unbind();
    }

    for (int i = 0; i < numBlurPasses; i++)
    {
        // Blur #2 Vertical
        hdrDownsample2Texture.Bind();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, hdrBlurTexture.GetTexID());
        hdrBlur.SetParameter("offset", 0.0f, 1.0f / (WINDOW_HEIGHT / 4.0f));
        hdrBlur.SetParameter("lodlevel", 0);

        fullscreenQuad.Render();

        hdrDownsample2Texture.Unbind();

        // Blur #2 Horizontal
        hdrBlur2Texture.Bind();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, hdrDownsample2Texture.GetTexID());
        hdrBlur.SetParameter("offset", 1.0f / (WINDOW_WIDTH / 4.0f), 0.0f);

        fullscreenQuad.Render();

        hdrBlur2Texture.Unbind();
    }
}

// ===================
// Final combined output
// Tonemap the GBufferFinal + Blur image.
// ===================
void World::DoFinalPass()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    Matrix pmat = Matrix::OrthoProjection(WINDOW_WIDTH, 0.0f, WINDOW_HEIGHT, 0.0f, -1, 1);
    Matrix vmat = Matrix::CreateIdentity();

    // Final output (Tonemapped)
    Shader* finalShader = &hdrTonemap;

    Shader::Bind(finalShader);

    // Bind final texture.
    GLuint texid;
    switch (currentTarget)
    {
    default:
    case BufferTarget::FINAL:
        texid = hdrBlur2Texture.GetTexID();
        break;
    case BufferTarget::DIFFUSE:
        texid = gbuffer.GetFinalID();
        break;
    case BufferTarget::POSITION:
        texid = gbuffer.GetPositionID();
        break;
    case BufferTarget::NORMAL:
        texid = gbuffer.GetNormalID();
        break;
    case BufferTarget::DEPTH:
        texid = gbuffer.GetDepthID();
        break;
    }
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texid);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gbuffer.GetFinalID());

    finalShader->SetParameter("projectionMatrix", &pmat[0][0]);
    finalShader->SetParameter("viewMatrix", &vmat[0][0]);
    finalShader->SetParameter("modelMatrix", &vmat[0][0]);
    finalShader->SetParameter("screenSize", WINDOW_WIDTH, WINDOW_HEIGHT);
    finalShader->SetParameter("tex", 0);
    finalShader->SetParameter("tex2", 1);
    finalShader->SetParameter("exposure", exposure);

    // Draw the quad.
    fullscreenQuad.Render();

    glDisable(GL_BLEND);
}