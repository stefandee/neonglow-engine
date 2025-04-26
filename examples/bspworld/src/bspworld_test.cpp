//------------------------------------------------------------------------------
// This file is part of NeonGlow Engine.                                      //
//                                                                            //
// NeonGlow Engine - software rendering generic 3D engine                     //
// Copyright (C) 1998, 1999, 2000, 2001 Stefan Dicu & Tudor Girba             //
//                                                                            //
// NeonGlow is free software; you can redistribute it                         //
// and/or modify it under the terms of the GNU General Public License as      //
// published by the Free Software Foundation; either version 2 of the         //
// License, or (at your option) any later version.                            //
//                                                                            //
// NeonGlow Engine is distributed in the hope that it will be useful, but     //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY //
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License   //
// for more details.                                                          //
//                                                                            //
// You should have received a copy of the GNU General Public License          //
// along with NeonGlow Engine; if not, write to the Free Software             //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA    //
//------------------------------------------------------------------------------

//
// This is a test of the bsp world creation
//

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
#include "tex.h"
#include "texmgr.h"
#include "bspworld.h"
#include "q3bspload.h"
#include "q2bspload.h"
#include <sdl\SDL.h>

CTexMgr    gTexMgr;
CTex*      gTex;
CBspWorld*    gWorld;
CCamera gCamera;
float gX = 0., gY = 0., gZ = 50.;

void Setup()
{
  CQ2BspLoad lBspLoad;

  gTexMgr.AddTex("data\\tex\\afrog.wal");
  gTex = gTexMgr.GetTex(0);

  gWorld = new CBspWorld();

  //
  // generate the world
  //
  // just a rectagle surface covered with a test texture
  //

  for(int i = 0; i < 6; i++)
  {
    gWorld->AddVertex(new CPoint3D(90.0 * sin(60 * i * 3.14 / 180), 0, 90.0 * cos(60 * i * 3.14 / 180)));
  }

  /*
  gWorld->AddVertex(new CPoint3D(90, 0, 90));
  gWorld->AddVertex(new CPoint3D(0, 0, 180));
  gWorld->AddVertex(new CPoint3D(-90, 0, -90));
  gWorld->AddVertex(new CPoint3D(90, 0, -90));
  gWorld->AddVertex(new CPoint3D(0, 0, -180));
  gWorld->AddVertex(new CPoint3D(90, 0, -90));
  */

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
  if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
          fprintf(stderr, "Couldn't initialize SDL: %s\n",SDL_GetError());
          exit(1);
  }
  atexit(SDL_Quit);

  video_bpp = 32;
  videoflags = SDL_SWSURFACE;

  //
  // Set 640x480 windowed video mode
  //
  if ( (screen=SDL_SetVideoMode(640,480,video_bpp,videoflags)) == NULL ) {
          fprintf(stderr, "Couldn't set 640x480x%d video mode: %s\n",
                                          video_bpp, SDL_GetError());
          exit(2);
  }

  //
  //  Init the engine stuff
  //
  Setup();

  /* Enable auto repeat for keyboard input */
  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

  /* Wait for a keystroke */
  done = 0;
  while ( !done ) {

  gWorld->Render();
  gRenderServer.Draw(screen);
  SDL_UpdateRect(screen, 0, 0, 0, 0);

  gRenderServer.EraseColorBuffer();
  gRenderServer.EraseZBuffer();

  CPoint3D lView, lFocus, lUp;
  CVector3D lN, lV, lU;

          /* Check for events */
          while ( SDL_PollEvent(&event) ) {
                  switch (event.type) {
                          case SDL_MOUSEBUTTONDOWN:
                                  gY += 8;
                                  gCamera.SetViewPoint(CPoint3D(gX, gY, gZ));
                                  gRenderServer.SetCamera(gCamera);
                                  break;
                          case SDL_KEYDOWN:
                                  cout << event.key.keysym.sym << endl;

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

                                      cout << lView.GetX() << "," << lView.GetY() << "," << lView.GetZ() << endl;

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

                                      cout << lView.GetX() << "," << lView.GetY() << "," << lView.GetZ() << endl;

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

                                      cout << lView.GetX() << "," << lView.GetY() << "," << lView.GetZ() << endl;

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

                                      cout << lView.GetX() << "," << lView.GetY() << "," << lView.GetZ() << endl;

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
                                  done = 1;
                                  break;

                          case SDL_MOUSEMOTION:
                              //cout << event.motion.xrel << "," << event.motion.yrel << endl;
                              break;
                                    
                          default:
                                  break;
                  }
          }
  }

  SDL_FreeSurface(screen);

  return(0);
}

