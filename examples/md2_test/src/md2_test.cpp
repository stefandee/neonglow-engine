//------------------------------------------------------------------------------
// This example loads and renders a Quake 2 MD2 model file.                   //
//                                                                            //
// This file is part of NeonGlow Engine.                                      //
//                                                                            //
// NeonGlow Engine - software rendering generic 3D engine                     //
// Copyright (C) 1998, 1999, 2000, 2001 Stefan Dicu & Tudor Girba             //
//                                                                            //
// Licensed under MIT license.                                                //
//------------------------------------------------------------------------------

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <exception>
#include <string>
#include <iostream>

#include <SDL2\SDL.h>

// engine includes
#include "math3d.h"
#include "geom.h"
#include "data3d.h"
#include "md2load.h"
#include "rserver.h"
//#include "objlib.h"
#include "tex.h"
#include "texmgr.h"

class InitError : public std::exception
{
    std::string msg;
public:
    InitError();
    InitError( const std::string & );
    virtual ~InitError() throw();
    virtual const char * what() const throw();
};

InitError::InitError() :
    exception(),
    msg( SDL_GetError() )
{
}

InitError::InitError( const std::string & m ) :
    exception(),
    msg( m )
{
}

InitError::~InitError() throw()
{
}

const char * InitError::what() const throw()
{
    return msg.c_str();
}

class SDL
{
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;

    SDL_Window * m_window;
    SDL_Surface* m_screenSurface;

    CObject3D* gObject;
    CTexMgr    gTexMgr;
    CTex*      gTex;
    CCamera gCamera;
    float gX = 0., gY = 0., gZ = 50.;

public:
    SDL( Uint32 flags = 0 );
    virtual ~SDL();
    void setup();
    void draw();
    void loop();
};

SDL::SDL( Uint32 flags )
{
    if ( SDL_Init( flags ) != 0 )
        throw InitError();

    // Create the window
    m_window = SDL_CreateWindow( "MD2 Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_BORDERLESS );

    if (!m_window)
    {
        throw InitError();
    }

    // Get the window surface
    m_screenSurface = SDL_GetWindowSurface( m_window );
}

SDL::~SDL()
{
    SDL_DestroyWindow( m_window );
    SDL_Quit();
}

void SDL::setup()
{
    CMd2Loader lLoader;

    gObject = lLoader.Load("data\\engine\\models\\tris.md2");
    gTexMgr.AddTex("data\\engine\\skins\\skin.wal");
    gTex = gTexMgr.GetTex(0);
    gObject->SetTexture(gTex);

    gRenderServer.EraseColorBuffer();
    gRenderServer.EraseZBuffer();

    gX = 0.;
    gY = 20.;
    gZ = 0.;

    gCamera.SetViewPoint(CPoint3D(gX, gY, gZ));
    //gCamera.SetFocusPoint(CPoint3D((bb.GetLowBound().GetX() + bb.GetHighBound().GetX())/2, (bb.GetLowBound().GetY() + bb.GetHighBound().GetY())/2, (bb.GetLowBound().GetZ() + bb.GetHighBound().GetZ())/2));
    gCamera.SetFocusPoint(CPoint3D(0., 0., 0.));
    gCamera.SetUpPoint(CPoint3D(0., 0., 100.));

    gRenderServer.SetCamera(gCamera);
}

void SDL::draw()
{
    gRenderServer.EraseColorBuffer();
    gRenderServer.EraseZBuffer();

    gObject->Draw();

    gRenderServer.Draw(m_screenSurface);

    //Update the surface
	SDL_UpdateWindowSurface( m_window );
}

void SDL::loop()
{
    SDL_Event event;

    bool done = false;

    while ( !done )
    {

        draw();

        CPoint3D lView, lFocus, lUp;
        CVector3D lN, lV, lU;

        /* Check for events */
        while ( SDL_PollEvent(&event) )
        {
            switch (event.type)
            {
            case SDL_MOUSEBUTTONDOWN:
                gY += 8;
                gCamera.SetViewPoint(CPoint3D(gX, gY, gZ));
                gRenderServer.SetCamera(gCamera);
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    // Q key
                    case 113:
                        lView = gCamera.GetViewPoint();
                        lU = gCamera.GetU();
                        lN = gCamera.GetN();

                        lView = CPoint3D(lView.GetX() + 2 * lU.GetX(), lView.GetY() + 2 * lU.GetY(), lView.GetZ() + 2 * lU.GetZ());

                        //gCamera.SetViewPoint(lView);

                        gCamera = CCamera(lView, gCamera.GetFocusPoint(), gCamera.GetUpPoint(), 0.0);

                        gRenderServer.SetCamera(gCamera);
                        break;

                    // E key
                    case 101:
                        lView = gCamera.GetViewPoint();
                        lU = gCamera.GetU();
                        lN = gCamera.GetN();

                        lView = CPoint3D(lView.GetX() - 2 * lU.GetX(), lView.GetY() - 2 * lU.GetY(), lView.GetZ() - 2 * lU.GetZ());

                        //gCamera.SetViewPoint(lView);
                        gCamera = CCamera(lView, gCamera.GetFocusPoint(), gCamera.GetUpPoint(), 0.0);
                        gRenderServer.SetCamera(gCamera);
                        break;

                    // W key
                    case 119:
                        lView  = gCamera.GetViewPoint();
                        lFocus = gCamera.GetFocusPoint();
                        lN     = gCamera.GetN();
                        lView  = CPoint3D(lView.GetX() - 2 * lN.GetX(), lView.GetY() - 2 * lN.GetY(), lView.GetZ() - 2 * lN.GetZ());
                        lFocus = CPoint3D(lFocus.GetX() - 2 * lN.GetX(), lFocus.GetY() - 2 * lN.GetY(), lFocus.GetZ() - 2 * lN.GetZ());

                        // cout << lView.GetX() << "," << lView.GetY() << "," << lView.GetZ() << endl;

                        gCamera = CCamera(lView, lFocus, gCamera.GetUpPoint(), 0.0);
                        //gCamera.SetViewPoint(lView);
                        //gCamera.SetFocusPoint(lFocus);
                        gRenderServer.SetCamera(gCamera);
                        break;

                    // S key
                    case 115:
                        lView = gCamera.GetViewPoint();
                        lFocus = gCamera.GetFocusPoint();
                        lN = gCamera.GetN();
                        lView = CPoint3D(lView.GetX() + 2 * lN.GetX(), lView.GetY() + 2 * lN.GetY(), lView.GetZ() + 2 * lN.GetZ());
                        lFocus = CPoint3D(lFocus.GetX() + 2 * lN.GetX(), lFocus.GetY() + 2 * lN.GetY(), lFocus.GetZ() + 2 * lN.GetZ());

                        // cout << lView.GetX() << "," << lView.GetY() << "," << lView.GetZ() << endl;

                        gCamera = CCamera(lView, lFocus, gCamera.GetUpPoint(), 0.0);
                        //gCamera.SetViewPoint(lView);
                        //gCamera.SetFocusPoint(lFocus);
                        gRenderServer.SetCamera(gCamera);
                        break;

                    // A key
                    case 97:
                        lView = gCamera.GetViewPoint();
                        lFocus = gCamera.GetFocusPoint();
                        lUp = gCamera.GetUpPoint();
                        lN = gCamera.GetU();
                        lView = CPoint3D(lView.GetX() + 5 * lN.GetX(), lView.GetY() + 5 * lN.GetY(), lView.GetZ() + 5 * lN.GetZ());
                        lFocus = CPoint3D(lFocus.GetX() + 5 * lN.GetX(), lFocus.GetY() + 5 * lN.GetY(), lFocus.GetZ() + 5 * lN.GetZ());
                        lUp = CPoint3D(lUp.GetX() + 5 * lN.GetX(), lUp.GetY() + 5 * lN.GetY(), lUp.GetZ() + 5 * lN.GetZ());

                        // cout << lView.GetX() << "," << lView.GetY() << "," << lView.GetZ() << endl;

                        gCamera = CCamera(lView, lFocus, lUp, 0.0);
                        //gCamera.SetViewPoint(lView);
                        //gCamera.SetFocusPoint(lFocus);
                        gRenderServer.SetCamera(gCamera);
                        break;

                    case 100:
                        lView = gCamera.GetViewPoint();
                        lFocus = gCamera.GetFocusPoint();
                        lUp = gCamera.GetUpPoint();
                        lN = gCamera.GetU();
                        lView = CPoint3D(lView.GetX() - 5 * lN.GetX(), lView.GetY() - 5 * lN.GetY(), lView.GetZ() - 5 * lN.GetZ());
                        lFocus = CPoint3D(lFocus.GetX() - 5 * lN.GetX(), lFocus.GetY() - 5 * lN.GetY(), lFocus.GetZ() - 5 * lN.GetZ());
                        lUp = CPoint3D(lUp.GetX() - 5 * lN.GetX(), lUp.GetY() - 5 * lN.GetY(), lUp.GetZ() - 5 * lN.GetZ());

                        // cout << lView.GetX() << "," << lView.GetY() << "," << lView.GetZ() << endl;

                        gCamera = CCamera(lView, lFocus, lUp, 0.0);
                        //gCamera.SetViewPoint(lView);
                        //gCamera.SetFocusPoint(lFocus);
                        gRenderServer.SetCamera(gCamera);
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
        SDL sdl( SDL_INIT_VIDEO | SDL_INIT_TIMER );

        sdl.setup();
        sdl.loop();

        return 0;
    }
    catch ( const InitError & err )
    {
        std::cerr << "Error while initializing SDL:  "
                  << err.what()
                  << std::endl;
    }

    return 1;
}
