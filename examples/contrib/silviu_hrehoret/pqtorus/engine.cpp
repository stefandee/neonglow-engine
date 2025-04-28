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

#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// engine includes
#include "math3d.h"
#include "geom.h"
#include "data3d.h"
#include "md2load.h"
#include "rserver.h"
//#include "objlib.h"
#include "tex.h"
#include "texmgr.h"
#include "bspworld.h"
#include "q3bspload.h"
#include "q2bspload.h"

bool       gLoaded = false;
CObject3D* gObject;
// *gO1, *gO2, *gO3, *g04;
CTexMgr    gTexMgr;
CTex*      gTex;
COLPQTorus*   pqtor;
int        gAngle = 0;
CBspWorld*    gWorld;
CCamera gCamera;
float gX = 0., gY = 0., gZ = 50.;

void Setup()
{
  CMd2Loader lLoader;
  CQ2BspLoad lBspLoad;

  //gObject = lLoader.Load("data\\models\\tris.md2");
  pqtor = new COLPQTorus();
  gObject = pqtor->Generate();
  gTexMgr.AddTex("data\\skins\\skin.wal"); gTex = gTexMgr.GetTex(0); gObject->SetTexture(gTex);

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

#include <sdl\SDL.h>

int main (int argc, char* argv[])
{
  SDL_Surface *screen;
  Uint8  video_bpp;
  Uint32 videoflags;
  Uint8 *buffer;
  int i, done;
  SDL_Event event;

  /* Initialize SDL */
  if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
          fprintf(stderr, "Couldn't initialize SDL: %s\n",SDL_GetError());
          exit(1);
  }
  atexit(SDL_Quit);

  video_bpp = 32;
  videoflags = SDL_SWSURFACE;

  /* Set 640x480 video mode */
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
                            
  /* Set the surface pixels and refresh! */
  /*
  if ( SDL_LockSurface(screen) < 0 ) {
          fprintf(stderr, "Couldn't lock the display surface: %s\n",
                                                  SDL_GetError());
          exit(2);
  }

  buffer=(Uint8 *)screen->pixels;
  for ( i=0; i<screen->h; ++i ) {
          memset(buffer,(i*255)/screen->h, screen->pitch);
          buffer += screen->pitch;
  }

  buffer=(Uint8 *)screen->pixels;
  for(int x = 0; x < 50; x++)
  {
    *((int*)(buffer + x * screen->pitch) + x) = 0xFF0000;
  }

  SDL_UnlockSurface(screen);
  */
  SDL_FillRect(screen, 0, 0x00007F);
  SDL_UpdateRect(screen, 0, 0, 0, 0);

  /* Wait for a keystroke */
  done = 0;
  while ( !done ) {

  gObject->Draw();
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

