//------------------------------------------------------------------------------
// This example loads and renders a Quake 2 MD2 model file.                   //
//                                                                            //
// This file is part of NeonGlow Engine.                                      //
//                                                                            //
// NeonGlow Engine - software rendering generic 3D engine                     //
// Copyright (C) 2003 Stefan Dicu                                             //
//                                                                            //
// Licensed under MIT license.                                                //
//------------------------------------------------------------------------------

#include <iostream>
#include <math.h>

// SDL
#include <SDL2\SDL.h>

// engine includes
#include "math3d.h"
#include "geom.h"
#include "data3d.h"
#include "rserver.h"
#include "tex.h"
#include "texmgr.h"
#include "prims.h"

// other
#include "SDLApp.h"
#include "SDLInitError.h"

class GouraudTestApp : public SDLApp
{
public:
    GouraudTestApp( Uint32 flags, const std::string& appName, int screenWidth = 640, int screenHeight = 480 );

public:
    void Setup() override;
    void Draw() override;
    void Loop() override;

private:
    void EraseZBuffer();

private:
    char*      gZBuffer;
    CTexMgr    gTexMgr;
    CTex*      gTex;

    CPoint2D p1 = CPoint2D(200., 100.), p2 = CPoint2D(150.,300.), p3 = CPoint2D(350.,350.);
};

GouraudTestApp::GouraudTestApp( Uint32 flags, const std::string& appName, int screenWidth, int screenHeight ) :
    SDLApp{flags, appName, screenWidth, screenHeight}
{
}

void GouraudTestApp::EraseZBuffer()
{
    for(int i = 0; i < RBufferH; i++)
    {
        for(int j = 0; j < RBufferW; j++)
        {
            *((short int*)gZBuffer + i * RBufferW + j) = 30000;
        }
    }
}

void GouraudTestApp::Setup()
{
    // load a texture
    gTexMgr.AddTex("data\\engine\\tex\\afrog.wal");
    gTex = gTexMgr.GetTex(0);

    // setup a dummy z-buffer
    gZBuffer     = new char[2 * RBufferW * RBufferH];
    EraseZBuffer();

    /*
    gRenderServer.EraseColorBuffer();
    gRenderServer.EraseZBuffer();

    gX = 128.;
    gY = -320.;
    gZ = 32.;

    gCamera = CCamera(CPoint3D(gX, gY, gZ), CPoint3D(0., 0., 0.), CPoint3D(0., 0., -100.), 0.0);

    gWorld->SetCamera(&gCamera);

    gRenderServer.SetCamera(gCamera);
    */
}

void GouraudTestApp::Draw()
{
    gRenderServer.EraseColorBuffer();
    // gRenderServer.EraseZBuffer();

    EraseZBuffer();

    ZGTexTriangle
    (
        (char*)m_screenSurface->pixels,
        m_screenSurface->pitch,
        p1.GetX(), p1.GetY(),
        p2.GetX(), p2.GetY(),
        p3.GetX(), p3.GetY(),
        gTex->GetData(0),
        gTex->GetWidth() * 4,
        0, 0,
        gTex->GetWidth(), 0,
        0, gTex->GetHeight(),
        gZBuffer,
        RBufferW * 2,
        20,
        20,
        20,
        gTex->GetWidth(),
        gTex->GetHeight(),
        0x00FF00,
        0x0000FF,
        0xFF0000
    );

    gRenderServer.Draw(m_screenSurface);

    //Update the surface
    SDL_UpdateWindowSurface( m_window );
}

void GouraudTestApp::Loop()
{
    SDL_Event event;

    bool done = false;

    while ( !done )
    {

        Draw();

        CPoint3D lView, lFocus, lUp;
        CVector3D lN, lV, lU;
        float lA, x, y;

        /* Check for events */
        while ( SDL_PollEvent(&event) )
        {
            switch (event.type)
            {
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                case SDLK_SPACE:
                    lA = 1. * 3.1415 / 180.0;

                    x = p1.GetX() - 200.;
                    y = p1.GetY() - 200.;
                    p1 = CPoint2D(x * cos(lA) - y * sin(lA) + 200., x * sin(lA) + y * cos(lA) + 200.);

                    x = p2.GetX() - 200.;
                    y = p2.GetY() - 200.;
                    p2 = CPoint2D(x * cos(lA) - y * sin(lA) + 200., x * sin(lA) + y * cos(lA) + 200.);

                    x = p3.GetX() - 200.;
                    y = p3.GetY() - 200.;
                    p3 = CPoint2D(x * cos(lA) - y * sin(lA) + 200., x * sin(lA) + y * cos(lA) + 200.);
                    break;

                case SDLK_ESCAPE:
                    done = true;
                    break;
                }
                break;

            case SDL_KEYUP:
                break;

            case SDL_QUIT:
                done = true;
                break;

            case SDL_MOUSEMOTION:
                //cout << event.motion.xrel << "," << event.motion.yrel << endl;
                break;

            default:
                break;
            }
        }
    }
}

int main( int argc, char * argv[] )
{
    try
    {
        GouraudTestApp app( SDL_INIT_VIDEO | SDL_INIT_TIMER, std::string("Gouraud Test") );

        app.Setup();
        app.Loop();

        return 0;
    }
    catch ( const SDLInitError & err )
    {
        std::cerr << "Error while initializing SDL:  "
                  << err.what()
                  << std::endl;
    }

    return 1;
}























/*
#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// engine includes
#include "math3d.h"
#include "geom.h"
#include "data3d.h"
#include "rserver.h"
#include "prims.h"
#include "tex.h"
#include "texmgr.h"
#include <sdl\SDL.h>

CTexMgr    gTexMgr;
CTex*      gTex;
char*      gZBuffer;

void Setup()
{
  // setup of the render server
  // gRenderServer.EraseColorBuffer();
  // gRenderServer.EraseZBuffer();

}

int main (int argc, char* argv[])
{
  SDL_Surface *screen;
  Uint8  video_bpp;
  Uint32 videoflags;
  Uint8 *buffer;
  int i, done;
  SDL_Event event;

  //
  // Initialize SDL
  //
  if ( SDL_Init(SDL_INIT_VIDEO) < 0 )
  {
    fprintf(stderr, "Couldn't initialize SDL: %s\n",SDL_GetError());
    exit(1);
  }
  atexit(SDL_Quit);

  video_bpp = 32;
  videoflags = SDL_SWSURFACE;

  //
  // Set 640x480 windowed video mode
  //
  if ( (screen=SDL_SetVideoMode(640,480,video_bpp,videoflags)) == NULL )
  {
    fprintf(stderr, "Couldn't set 640x480x%d video mode: %s\n", video_bpp, SDL_GetError());
    exit(2);
  }

  //
  //  Init the engine stuff
  //
  Setup();

  // Enable auto repeat for keyboard input
  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

  CPoint2D p1(200., 100.), p2(150.,300.), p3(350.,350.);
  float lA = 0.0, x, y;

  // Wait for a keystroke
  done = 0;

  while ( !done )
  {
    SDL_FillRect(screen, 0, 0);

    for(int i = 0; i < RBufferH; i++)
    {
      for(int j = 0; j < RBufferW; j++)
      {
        *((short int*)gZBuffer + i * RBufferW + j) = 30000;
      }
    }

    ZGTexTriangle
    (
      (char*)screen->pixels,
      screen->pitch,
      p1.GetX(), p1.GetY(),
      p2.GetX(), p2.GetY(),
      p3.GetX(), p3.GetY(),
      gTex->GetData(0),
      gTex->GetWidth() * 4,
      0, 0,
      gTex->GetWidth(), 0,
      0, gTex->GetHeight(),
      gZBuffer,
      RBufferW * 2,
      20,
      20,
      20,
      gTex->GetWidth(),
      gTex->GetHeight(),
      0x00FF00,
      0x0000FF,
      0xFF0000
    );

    SDL_UpdateRect(screen, 0, 0, 0, 0);

    // Check for events
    while ( SDL_PollEvent(&event) )
    {
      switch (event.type)
      {
        case SDL_MOUSEBUTTONDOWN:
          break;

        case SDL_KEYDOWN:
          lA = 1. * 3.1415 / 180.0;

          x = p1.GetX() - 200.;
          y = p1.GetY() - 200.;
          p1 = CPoint2D(x * cos(lA) - y * sin(lA) + 200., x * sin(lA) + y * cos(lA) + 200.);

          x = p2.GetX() - 200.;
          y = p2.GetY() - 200.;
          p2 = CPoint2D(x * cos(lA) - y * sin(lA) + 200., x * sin(lA) + y * cos(lA) + 200.);

          x = p3.GetX() - 200.;
          y = p3.GetY() - 200.;
          p3 = CPoint2D(x * cos(lA) - y * sin(lA) + 200., x * sin(lA) + y * cos(lA) + 200.);
          break;

        case SDL_KEYUP:
          break;

        case SDL_QUIT:
          done = 1;
          break;

        case SDL_MOUSEMOTION:
          break;

        default:
          break;
      }
    }
  }

  SDL_FreeSurface(screen);

  return(0);
}

*/
