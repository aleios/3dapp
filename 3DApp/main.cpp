#include <iostream>
#include <time.h>
#include <GL/glew.h>

#include "Game.hpp"

#include "Matrix.hpp"

#include "SceneController.hpp"

int main(int argc, char** argv)
{
    srand(static_cast<unsigned int>(time(0)));

    // Enable Flush to Zero (FTZ) and Denormals are Zero (DAZ) to reduce impact caused by a denormalized float exception.
    // We don't need numbers that are very close to 0 and want to avoid slow microcode from executing.
    _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);

    // Set the FPU State.
    unsigned int cState;
    int err;

#ifndef _WIN64
    // Set the precision to 24 bits. Not supported in x64.
    err = _controlfp_s(&cState, _PC_24, _MCW_PC);
    if (err)
    {
        _controlfp_s(&cState, _PC_24, cState);
    }
#endif

    // Set the rounding mode to nearest.
    err = _controlfp_s(&cState, _RC_NEAR, _MCW_RC);
    if (err)
    {
        _controlfp_s(&cState, _RC_NEAR, cState);
    }

    // Run the game
    Game game;
    game.Run();

    return 0;
}