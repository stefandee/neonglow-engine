#include "SDLApp.h"

#include "SDLInitError.h"

SDLApp::SDLApp( Uint32 flags, const std::string& appName, int screenWidth, int screenHeight )
{
    if ( SDL_Init( flags ) != 0 )
        throw SDLInitError();

    // Create the window
    m_window = SDL_CreateWindow( appName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_BORDERLESS );

    if (!m_window)
    {
        throw SDLInitError();
    }

    // Get the window surface
    m_screenSurface = SDL_GetWindowSurface( m_window );
}

SDLApp::~SDLApp()
{
    SDL_DestroyWindowSurface( m_window );
    SDL_DestroyWindow( m_window );

    m_screenSurface = nullptr;
    m_window = nullptr;

    SDL_Quit();
}

