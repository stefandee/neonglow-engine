//----------------------------------------------------------------------------//
// This example shows how to manually build a BSP world                       //
//                                                                            //
// This file is part of NeonGlow Engine.                                      //
//                                                                            //
// NeonGlow Engine - software rendering generic 3D engine                     //
// Copyright (C) 2003 Stefan Dicu                                             //
//                                                                            //
// Licensed under MIT license.                                                //
//----------------------------------------------------------------------------//

#include <iostream>
#include <math.h>

// SDL
#include <SDL2\SDL.h>

// engine includes
#include "math3d.h"
#include "geom.h"
#include "data3d.h"
#include "md2load.h"
#include "rserver.h"
#include "tex.h"
#include "texmgr.h"
#include "q2bspload.h"
#include "bspworld.h"

// other
#include "SDLApp.h"
#include "SDLInitError.h"

class BSPWorldTestApp : public SDLApp
{
public:
    BSPWorldTestApp( Uint32 flags, const std::string& appName, int screenWidth = 640, int screenHeight = 480 );

public:
    void Setup() override;
    void Draw() override;
    void Loop() override;

private:
    CBspWorld* gWorld;
    CTexMgr    gTexMgr;
    CTex*      gTex;
};

BSPWorldTestApp::BSPWorldTestApp( Uint32 flags, const std::string& appName, int screenWidth, int screenHeight ) :
    SDLApp{flags, appName, screenWidth, screenHeight}
{
}

void BSPWorldTestApp::Setup()
{
  gTexMgr.AddTex("data\\engine\\tex\\afrog.wal");
  gTex = gTexMgr.GetTex(0);

  gWorld = new CBspWorld();

  //
  // generate the world
  //
  // just a polygonal surface covered with a test texture
  //
  for(int i = 0; i < 6; i++)
  {
    gWorld->AddVertex(new CPoint3D(90.0 * sin(60 * i * 3.14 / 180), 0, 90.0 * cos(60 * i * 3.14 / 180)));
  }

  gWorld->AddBspEdge(0, 1);
  gWorld->AddBspEdge(1, 2);
  gWorld->AddBspEdge(2, 3);
  gWorld->AddBspEdge(3, 4);
  gWorld->AddBspEdge(4, 5);
  gWorld->AddBspEdge(5, 0);

  gWorld->AddPlane(new CPlane(0, 1., 0, 0));

  gWorld->AddFaceEdge(0);
  gWorld->AddFaceEdge(1);
  gWorld->AddFaceEdge(2);
  gWorld->AddFaceEdge(3);
  gWorld->AddFaceEdge(4);
  gWorld->AddFaceEdge(5);

  CBspFace* lFace = new CBspFace();

  lFace->SetTexInfo(0);
  lFace->SetEdgeCount(6);
  lFace->SetEdgeIndex(0);
  lFace->SetPlaneIndex(0);
  lFace->SetPlaneSide(0);

  gWorld->AddBspFace(lFace);

  CBspTex* lBspTex = new CBspTex();

  lBspTex->SetTex(gTex);
  lBspTex->SetUAxis(CPoint3D(-1, 0, 0));
  lBspTex->SetVAxis(CPoint3D(0, 0, -1));
  //lBspTex->SetUOffset(gTex->GetWidth() - 1);
  //lBspTex->SetVOffset(gTex->GetHeight() - 1);
  lBspTex->SetUOffset(320);
  lBspTex->SetVOffset(256);

  gWorld->AddBspTex(lBspTex);


  gRenderServer.EraseColorBuffer();
  gRenderServer.EraseZBuffer();

  //gX = 96.;
  //gY = -224.;
  //gZ = 24.;
  gX = 0.;
  gY = 20.;
  gZ = 0.;

  gCamera.SetViewPoint(CPoint3D(gX, gY, gZ));
  gCamera.SetFocusPoint(CPoint3D(0., 0., 0.));
  gCamera.SetUpPoint(CPoint3D(0., 0., -1000.));
  gWorld->SetCamera(&gCamera);

  gRenderServer.SetCamera(gCamera);

    /*
  gWorld = CQ2BspLoad().Load("data\\engine\\maps\\base1.bsp", &gTexMgr);

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

void BSPWorldTestApp::Draw()
{
    gRenderServer.EraseColorBuffer();
    gRenderServer.EraseZBuffer();

    gWorld->Render();

    gRenderServer.Draw(m_screenSurface);

    //Update the surface
	SDL_UpdateWindowSurface( m_window );
}

void BSPWorldTestApp::Loop()
{
    SDL_Event event;

    bool done = false;

    while ( !done )
    {

        Draw();

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
                    case SDLK_q:
                        lView = gCamera.GetViewPoint();
                        lU = gCamera.GetU();
                        lN = gCamera.GetN();

                        lView = CPoint3D(lView.GetX() + 5 * lU.GetX(), lView.GetY() + 5 * lU.GetY(), lView.GetZ() + 5 * lU.GetZ());

                        //gCamera.SetViewPoint(lView);

                        gCamera = CCamera(lView, gCamera.GetFocusPoint(), gCamera.GetUpPoint(), 0.0);

                        gRenderServer.SetCamera(gCamera);
                        break;

                    // E key
                    case SDLK_e:
                        lView = gCamera.GetViewPoint();
                        lU = gCamera.GetU();
                        lN = gCamera.GetN();

                        lView = CPoint3D(lView.GetX() - 5 * lU.GetX(), lView.GetY() - 5 * lU.GetY(), lView.GetZ() - 5 * lU.GetZ());

                        //gCamera.SetViewPoint(lView);
                        gCamera = CCamera(lView, gCamera.GetFocusPoint(), gCamera.GetUpPoint(), 0.0);
                        gRenderServer.SetCamera(gCamera);
                        break;

                    // W key
                    case SDLK_w:
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
                    case SDLK_s:
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
                    case SDLK_a:
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

                    case SDLK_d:
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
        BSPWorldTestApp app( SDL_INIT_VIDEO | SDL_INIT_TIMER, std::string("BSP World Test") );

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
