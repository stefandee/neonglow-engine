//------------------------------------------------------------------------------
// This file is part of NeonGlow Engine.                                      //
//                                                                            //
// NeonGlow Engine - software rendering generic 3D engine                     //
// Copyright (C) 2003 Stefan Dicu                                             //
//                                                                            //
// Licensed under MIT license.                                                //
//------------------------------------------------------------------------------

#include "rserver.h"
#include <math.h>
#include "math3d.h"
#include "prims.h"
#include "string.h"
#include "bspworld.h"
#include <iostream>

CRServer gRenderServer;

float    gEyeZ = -10., gScreenX = 320., gScreenY = 240.;

CRServer::CRServer()
{
    //mColorBuffer = new char[4 * RBufferW * RBufferH];
    Uint32 rmask, gmask, bmask, amask;

    /* SDL interprets each pixel as a 32-bit number, so our masks must depend
       on the endianness (byte order) of the machine */

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x00000000;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0x00000000;
#endif

    mColorBuffer = SDL_CreateRGBSurface(SDL_SWSURFACE, RBufferW, RBufferH, 32, rmask, gmask, bmask, amask);

    mZBuffer     = new char[2 * RBufferW * RBufferH];
}
//-----------------------------------------------------------------

CRServer::~CRServer()
{
    SDL_FreeSurface(mColorBuffer);
    delete [] mZBuffer;
}
//-----------------------------------------------------------------

void CRServer::RenderTriangle(std::vector<CVertex*>& _points)
{
    CPoint2D lProj[8];

    for(int i = 0; i < 3; i++)
    {
        CPoint3D lP = _points[i]->GetOriginal();

        lP = mMatrix.Transform(lP);

        float lZScale = gEyeZ / (gEyeZ + lP.GetZ());

        lProj[i] = CPoint2D(gScreenX + lP.GetX() * lZScale, gScreenY + lP.GetY() * lZScale);
        //lProj[i] = CPoint2D(160 + 4 * lP.GetY(), 100 - 4 * lP.GetZ());
    }

    PointLine(lProj[0], lProj[1]);
    PointLine(lProj[1], lProj[2]);
    PointLine(lProj[2], lProj[0]);
}
//-----------------------------------------------------------------

void CRServer::RenderTexTriangle(std::vector<CVertex*>& _points, std::vector<CTexVertex*> _tpoints, CTex* _tex)
{
    // TODO: implement it, but before we need a tex triangle routine and
    // a back-buffer flipper
    CPoint2D lProj[8];
    float    lZ[8];

    std::vector<CPoint3D*>   _outpoints, _points1;
    std::vector<CTexVertex*> _outtpoints;

    CPlane lNearPlane(0., 0., 1., -5.);

    for(unsigned int i = 0; i < _points.size(); i++)
    {
        CPoint3D lP = _points[i]->GetOriginal();
        lP = mCameraMatrix.Transform(lP);
        _points1.push_back(new CPoint3D(lP));
        //_outpoints.push_back(new CPoint3D(lP));
        //_outtpoints.push_back(new CTexVertex(*_tpoints[i]));
    }

    int result = ClipToPlane(_points1, _tpoints, lNearPlane, _outpoints, _outtpoints);

    if (result == 0)
    {
        return;
    }

    for(unsigned int i = 0; i < _outpoints.size(); i++)
    {
        CPoint3D lP = *_outpoints[i];
        float lZScale;

        //cout << lP.GetX() << ", " << lP.GetY() << ", " << lP.GetZ() << " - " << endl;
        //lP = mCameraMatrix.Transform(lP);
        //cout << lP.GetX() << ", " << lP.GetY() << ", " << lP.GetZ() << " - ";

        lZ[i] = lP.GetZ();

        lZScale = -8 * gEyeZ / (-gEyeZ + lP.GetZ());

        lProj[i] = CPoint2D(gScreenX + lP.GetX() * lZScale, gScreenY + lP.GetY() * lZScale);
        //lProj[i] = CPoint2D(160 + 4 * lP.GetY(), 100 - 4 * lP.GetZ());
        //cout << lProj[i].GetX() << ", " << lProj[i].GetY() << endl;
    }

    // a tex triangle should go here
    //cout << "Rendering tex tri" << endl;
    //cout << _tpoints[0]->GetU() << "," << _tpoints[0]->GetV() << endl;
    //cout << _tpoints[1]->GetU() << "," << _tpoints[1]->GetV() << endl;
    //cout << _tpoints[2]->GetU() << "," << _tpoints[2]->GetV() << endl;
    //cout << _tex->GetWidth() * 4 << endl;

    //cout << "ztex" << endl;

    /*
    if ( SDL_LockSurface(mColorBuffer) < 0 )
    {
            fprintf(stderr, "Couldn't lock the color buffer surface: %s\n",
                                                    SDL_GetError());
            exit(2);
    }
    */


    for(unsigned int i = 2; i < _outpoints.size(); i++)
    {
        /*
        cout << _outtpoints[i]->GetU() << "," << _outtpoints[i]->GetV() << "...";

        if (i < 3)
        {
          cout << _tpoints[i]->GetU() << "," << _tpoints[i]->GetV() << endl;
        }
        else
        {
          cout << endl;
        }
        */

        ZTexTriangle(
            (char*)mColorBuffer->pixels,
            mColorBuffer->pitch,
            (int)lProj[0].GetX(), (int)lProj[0].GetY(),
            (int)lProj[i-1].GetX(), (int)lProj[i-1].GetY(),
            (int)lProj[i].GetX(), (int)lProj[i].GetY(),
            _tex->GetData(0),
            _tex->GetWidth() * 4,
            _outtpoints[0]->GetU(), _outtpoints[0]->GetV(),
            _outtpoints[i-1]->GetU(), _outtpoints[i-1]->GetV(),
            _outtpoints[i]->GetU(), _outtpoints[i]->GetV(),
            mZBuffer,
            RBufferW * 2,
            lZ[0],
            lZ[i-1],
            lZ[i],
            _tex->GetWidth(),
            _tex->GetHeight()
        );
    }

    //SDL_UnlockSurface(mColorBuffer);
}
//-----------------------------------------------------------------

void CRServer::RenderBspTexTriangle(std::vector<CPoint3D*>& _points, std::vector<CTexVertex*> _tpoints, CTex* _tex, CBspTex* _bspTex)
{
    CPoint2D lProj[64];
    float    lZ[64];
    std::vector<CPoint3D*>   _outpoints, _points1;
    std::vector<CTexVertex*> _outtpoints;

    CPlane lNearPlane(0., 0., 1., -1.);

    for(unsigned int i = 0; i < _points.size(); i++)
    {
        CPoint3D lP = *_points[i];
        lP = mCameraMatrix.Transform(lP);
        _points1.push_back(new CPoint3D(lP));
    }

    int result = ClipToPlane(_points1, _tpoints, lNearPlane, _outpoints, _outtpoints);

    if (result == 0)
    {
        return;
    }

    for(int i = 0; i < (int)_outpoints.size(); i++)
    {
        float lYScale;

        CPoint3D lP = *_outpoints[i];
        lZ[i] = lP.GetZ();

        lYScale = -gEyeZ * 16 / (-gEyeZ + lP.GetZ());

        lProj[i] = CPoint2D(gScreenX + lP.GetX() * lYScale, gScreenY + lP.GetY() * lYScale);
    }

    //_tpoints[0]->SetU(0); _tpoints[0]->SetV(0);
    //_tpoints[1]->SetU(0); _tpoints[1]->SetV(_tex->GetHeight() - 1);
    //_tpoints[2]->SetU(_tex->GetWidth() - 1); _tpoints[2]->SetV(0);

    for(int i = 0; i < (int)_outtpoints.size(); i++)
    {
        float lU0 = _outtpoints[i]->GetU();
        float lV0 = _outtpoints[i]->GetV();

        if (lU0 < 0 || lV0 < 0 || lU0 > 1.0 || lV0 > 1.0)
        {
            //cout << "(u,v) -> (" << _outtpoints[i]->GetU() << "," << _outtpoints[i]->GetV() << ")" << "..." << _tex->GetWidth() << "," << _tex->GetHeight() << endl;
        }

        if (lU0 < 0)
        {
            _outtpoints[i]->SetU(-lU0);
        }

        if (lV0 < 0)
        {
            _outtpoints[i]->SetV(-lV0);
        }

        /*

        lU0 = _outtpoints[i]->GetU();
        lV0 = _outtpoints[i]->GetV();

        if (lU0 > _tex->GetWidth() - 1)
        {
          _outtpoints[i]->SetU(lU0 / (float)_tex->GetWidth());
        }

        if (lV0 > _tex->GetHeight() - 1)
        {
          _outtpoints[i]->SetV(lV0 / (float)_tex->GetHeight());
        }
        */
    }

    for(int i = 2; i < (int)_outpoints.size(); i++)
    {

        /*
        cout << "ztex traingle...";
        cout << "Ztex" << endl;
        cout << "points " << "(" << _points[0]->GetX() << ", " << _points[0]->GetY() << ", " << _points[0]->GetZ() << endl;
        cout << "points " << "(" << _points[1]->GetX() << ", " << _points[1]->GetY() << ", " << _points[1]->GetZ() << endl;
        cout << "points " << "(" << _points[2]->GetX() << ", " << _points[2]->GetY() << ", " << _points[2]->GetZ() << endl;
        cout << "tpoints " << "(" << _tpoints[0]->GetU() << "," << _tpoints[0]->GetV() << ")" << endl;
        cout << "tpoints " << "(" << _tpoints[1]->GetU() << "," << _tpoints[1]->GetV() << ")" << endl;
        cout << "tpoints " << "(" << _tpoints[2]->GetU() << "," << _tpoints[2]->GetV() << ")" << endl;
        cout << "proj: (" << lProj[0].GetX() << ", " << lProj[0].GetY() << ")" << endl;
        cout << "proj: (" << lProj[1].GetX() << ", " << lProj[1].GetY() << ")" << endl;
        cout << "proj: (" << lProj[2].GetX() << ", " << lProj[2].GetY() << ")" << endl;
        cout << "Z: " << lZ[0] << ", " << lZ[1] << ", " << lZ[2] << endl;
        */

        ZTexTriangle(
            (char*)mColorBuffer->pixels,
            mColorBuffer->pitch,
            (int)lProj[0].GetX(), (int)lProj[0].GetY(),
            (int)lProj[i-1].GetX(), (int)lProj[i-1].GetY(),
            (int)lProj[i].GetX(), (int)lProj[i].GetY(),
            _tex->GetData(0),
            _tex->GetWidth() * 4,
            _outtpoints[0]->GetU() * _tex->GetWidth() - 1, _outtpoints[0]->GetV() * _tex->GetHeight() - 1,
            _outtpoints[i-1]->GetU() * _tex->GetWidth() - 1, _outtpoints[i-1]->GetV() * _tex->GetHeight() - 1,
            _outtpoints[i]->GetU() * _tex->GetWidth() - 1, _outtpoints[i]->GetV() * _tex->GetHeight() - 1,
            mZBuffer,
            RBufferW * 2,
            lZ[0],
            lZ[i-1],
            lZ[i],
            _tex->GetWidth(),
            _tex->GetHeight()
        );
    }

    //PointLine(lProj[0], lProj[1]);
    //PointLine(lProj[1], lProj[2]);
    //PointLine(lProj[2], lProj[0]);
}
//-----------------------------------------------------------------

void CRServer::RenderSurface(CSurface* _surf)
{
}
//-----------------------------------------------------------------

void CRServer::SetupFrustrum()
{
}
//-----------------------------------------------------------------

int CRServer::ClipToFrustrum()
{
    return 1;
}
//-----------------------------------------------------------------

// Description : Clips a polygon to a plane
// Param       :
// Result      :
// Comments    : taken from a Michael Abrash DDJ article
int CRServer::ClipToPlane(std::vector<CPoint3D*>& _points, std::vector<CTexVertex*>& _tpoints,
                          CPlane _plane,
                          std::vector<CPoint3D*>& _outpoints, std::vector<CTexVertex*>& _outtpoints)
{
    int     i, nextvert, curin, nextin;
    double  curdot, nextdot, scale;

    curdot = _plane.GetNormal() * CVector3D(_points[0]->GetX(), _points[0]->GetY(), _points[0]->GetZ());
    curin = (curdot >= _plane.GetDist());

    for (i = 0; i < (int)_points.size(); i++)
    {
        nextvert = (i + 1) % _points.size();

        // Keep the current vertex if it's inside the plane
        if (curin)
        {
            _outpoints.push_back(new CPoint3D(*_points[i]));
            _outtpoints.push_back(new CTexVertex(*_tpoints[i]));
        }

        nextdot = _plane.GetNormal() * CVector3D(_points[nextvert]->GetX(), _points[nextvert]->GetY(), _points[nextvert]->GetZ());
        nextin = (nextdot >= _plane.GetDist());

        // Add a clipped vertex if one end of the current edge is
        // inside the plane and the other is outside
        if (curin != nextin)
        {
            scale = (_plane.GetDist() - curdot) / (nextdot - curdot);

            _outpoints.push_back(new CPoint3D(_points[i]->GetX() + (_points[nextvert]->GetX() - _points[i]->GetX()) * scale,
                                              _points[i]->GetY() + (_points[nextvert]->GetY() - _points[i]->GetY()) * scale,
                                              _points[i]->GetZ() + (_points[nextvert]->GetZ() - _points[i]->GetZ()) * scale));

            _outtpoints.push_back(new CTexVertex(_tpoints[i]->GetU() + (_tpoints[nextvert]->GetU() - _tpoints[i]->GetU()) * scale,
                                                 _tpoints[i]->GetV() + (_tpoints[nextvert]->GetV() - _tpoints[i]->GetV()) * scale));
        }

        curdot = nextdot;
        curin = nextin;
    }

    if (_outpoints.size() < 3)
    {
        return 0;
    }

    return 1;
}
//-----------------------------------------------------------------

void CRServer::EraseZBuffer()
{
    for(int i = 0; i < RBufferH; i++)
    {
        for(int j = 0; j < RBufferW; j++)
        {
            *((short int*)mZBuffer + i * RBufferW + j) = 30000;
        }
    }
//  memset(mZBuffer, 0x70, 2 * RBufferH * RBufferW);
}
//-----------------------------------------------------------------

void CRServer::EraseColorBuffer()
{
    SDL_FillRect(mColorBuffer, 0, 0);
    //memset(mColorBuffer, 0x80, 4 * RBufferH * RBufferW);
}
//-----------------------------------------------------------------

void CRServer::Draw(SDL_Surface* _surface)
{
    SDL_BlitSurface(mColorBuffer, 0, _surface, 0);
}
//-----------------------------------------------------------------

void CRServer::SetCamera(CCamera _v)
{
    mCamera = _v;
    //cout << "computing matrix " << endl;
    mCameraMatrix = mCamera.ComputeMatrix();
}
//-----------------------------------------------------------------

void CRServer::PointLine(CPoint2D _p1, CPoint2D _p2)
{
    //LineNC((int)_p1.GetX(), (int)_p1.GetY(), (int)_p2.GetX(), (int)_p2.GetY());
}


