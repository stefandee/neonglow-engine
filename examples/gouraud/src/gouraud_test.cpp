//------------------------------------------------------------------------------
// This example tests triangle texturing and lighting using Gouraud model.    //
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

    void UseLighting(bool v);
    void SetUVRepeat(CPoint2D v) { uvRepeat = v; }

private:
    void EraseZBuffer();

private:
    char*      gZBuffer;
    CTexMgr    gTexMgr;
    CTex*      gTex;
    bool       useLighting = true;
    CPoint2D  uvRepeat;

    CPoint2D p1 = CPoint2D(20., 15.), p2 = CPoint2D(620.,15.), p3 = CPoint2D(620.,465.), p4 = CPoint2D(20.f, 465.f);
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

    if (useLighting)
    {
        ZGTexTriangle
        (
            (char*)gRenderServer.mColorBuffer->pixels,
            gRenderServer.mColorBuffer->pitch,
            p1.GetX(), p1.GetY(),
            p2.GetX(), p2.GetY(),
            p3.GetX(), p3.GetY(),
            gTex->GetData(0),
            gTex->GetWidth() * 4,
            0, 0,
            gTex->GetWidth() - 1, 0,
            gTex->GetWidth() - 1, gTex->GetHeight() - 1,
            gZBuffer,
            RBufferW * 2,
            100,
            100,
            100,
            gTex->GetWidth(),
            gTex->GetHeight(),
            0xFF0000,
            0xFFFF00,
            0x0000FF
        );

        ZGTexTriangle
        (
            (char*)gRenderServer.mColorBuffer->pixels,
            gRenderServer.mColorBuffer->pitch,
            p3.GetX(), p3.GetY(),
            p4.GetX(), p4.GetY(),
            p1.GetX(), p1.GetY(),
            gTex->GetData(0),
            gTex->GetWidth() * 4,
            gTex->GetWidth() - 1, gTex->GetHeight() - 1,
            0, gTex->GetHeight() - 1,
            0, 0,
            gZBuffer,
            RBufferW * 2,
            20,
            20,
            20,
            gTex->GetWidth(),
            gTex->GetHeight(),
            0x0000FF,
            0xFFFF00,
            0xFF0000
        );
    }
    else
    {
        ZTexTriangle
        (
            (char*)gRenderServer.mColorBuffer->pixels,
            gRenderServer.mColorBuffer->pitch,
            p1.GetX(), p1.GetY(),
            p2.GetX(), p2.GetY(),
            p3.GetX(), p3.GetY(),
            gTex->GetData(0),
            gTex->GetWidth() * 4,
            0, 0,
            uvRepeat.GetX() * gTex->GetWidth() - 1, 0,
            uvRepeat.GetX() * gTex->GetWidth() - 1, uvRepeat.GetY() * gTex->GetHeight() - 1,
            gZBuffer,
            RBufferW * 2,
            100,
            100,
            100,
            gTex->GetWidth(),
            gTex->GetHeight()
        );

        ZTexTriangle
        (
            (char*)gRenderServer.mColorBuffer->pixels,
            gRenderServer.mColorBuffer->pitch,
            p3.GetX(), p3.GetY(),
            p4.GetX(), p4.GetY(),
            p1.GetX(), p1.GetY(),
            gTex->GetData(0),
            gTex->GetWidth() * 4,
            uvRepeat.GetX() * gTex->GetWidth() - 1, uvRepeat.GetY() * gTex->GetHeight() - 1,
            0, uvRepeat.GetY() * gTex->GetHeight() - 1,
            0, 0,
            gZBuffer,
            RBufferW * 2,
            20,
            20,
            20,
            gTex->GetWidth(),
            gTex->GetHeight()
        );
    }

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

                    x = p1.GetX() - 320.;
                    y = p1.GetY() - 240.;
                    p1 = CPoint2D(x * cos(lA) - y * sin(lA) + 320., x * sin(lA) + y * cos(lA) + 240.);

                    x = p2.GetX() - 320.;
                    y = p2.GetY() - 240.;
                    p2 = CPoint2D(x * cos(lA) - y * sin(lA) + 320., x * sin(lA) + y * cos(lA) + 240.);

                    x = p3.GetX() - 320.;
                    y = p3.GetY() - 240.;
                    p3 = CPoint2D(x * cos(lA) - y * sin(lA) + 320., x * sin(lA) + y * cos(lA) + 240.);

                    x = p4.GetX() - 320.;
                    y = p4.GetY() - 240.;
                    p4 = CPoint2D(x * cos(lA) - y * sin(lA) + 320., x * sin(lA) + y * cos(lA) + 240.);
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

void GouraudTestApp::UseLighting(bool v)
{
    useLighting = v;
}

int main( int argc, char * argv[] )
{
    try
    {
        GouraudTestApp app( SDL_INIT_VIDEO | SDL_INIT_TIMER, std::string("Gouraud Test") );

        app.UseLighting(false);
        app.SetUVRepeat(CPoint2D(2.f, 2.f));
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
