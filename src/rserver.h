//------------------------------------------------------------------------------
// This file is part of NeonGlow Engine.                                      //
//                                                                            //
// NeonGlow Engine - software rendering generic 3D engine                     //
// Copyright (C) 2003 Stefan Dicu                                             //
//                                                                            //
// Licensed under MIT license.                                                //
//------------------------------------------------------------------------------

// Quick and Dirty - Rendering Server
// the hub to show 3D to screen - all rendering pass thru this class

#ifndef RSERVERH
#define RSERVERH

#include "data3d.h"
#include "camera.h"
#include <SDL2\SDL.h>

class CBspTex;

const long RBufferW = 640;
const long RBufferH = 480;

class CRServer
{
public: // c-d
    CRServer();
    ~CRServer();

public: // rendering
    void RenderTriangle(std::vector<CVertex*>& _points); // temporary
    //void RenderTexTriangle(vector<CVertex*>& _points, CTex* _tex); // temporary
    void RenderTexTriangle(std::vector<CVertex*>& _points, std::vector<CTexVertex*> _tpoints, CTex* _tex);
    void RenderBspTexTriangle(std::vector<CPoint3D*>& _points, std::vector<CTexVertex*> _tpoints, CTex* _tex, CBspTex* _bspTex);
    void RenderSurface(CSurface* _surf);

    void Draw(SDL_Surface* _surface);

public: // matrixes
    void SetMatrix(CMatrix4 _v)
    {
        mMatrix = _v;
    };
    void SetCamera(CCamera _c);

public: // erasers
    void EraseZBuffer();
    void EraseColorBuffer();

protected:
    void SetupFrustrum();
    int ClipToFrustrum();
    int ClipToPlane(std::vector<CPoint3D*>& _points, std::vector<CTexVertex*>& _tpoints,
                    CPlane _plane,
                    std::vector<CPoint3D*>& _outpoints, std::vector<CTexVertex*>& _outtpoints);

private:
    char* mZBuffer; // the z-buffer
    //char* mColorBuffer; // this is the color buffer - sort of like the back buffer
    SDL_Surface* mColorBuffer;
    CMatrix4 mMatrix, mCameraMatrix;
    CCamera mCamera;

    void PointLine(CPoint2D _p1, CPoint2D _p2);
};

extern CRServer gRenderServer;

#endif
