#ifndef SDLAPP_H
#define SDLAPP_H

#include <string>

#include <SDL2\SDL.h>

#include "camera.h"

class SDLApp
{
public:
    SDLApp( Uint32 flags, const std::string& appName, int screenWidth = 640, int screenHeight = 480 );
    virtual ~SDLApp();
    virtual void Setup() = 0;
    virtual void Draw() = 0;
    virtual void Loop() = 0;

protected:
    SDL_Window * m_window;
    SDL_Surface* m_screenSurface;

    CCamera gCamera;
    float gX = 0., gY = 0., gZ = 50.;

private:
};

#endif // SDLAPP_H
