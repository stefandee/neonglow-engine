//------------------------------------------------------------------------------
// This file is part of NeonGlow Engine.                                      //
//                                                                            //
// NeonGlow Engine - software rendering generic 3D engine                     //
// Copyright (C) 2003 Stefan Dicu                                             //
//                                                                            //
// Licensed under MIT license.                                                //
//------------------------------------------------------------------------------

#pragma hdrstop

#include "CTriangleSurface.h"
#include "rserver.h"
//---------------------------------------------------------------------------

CTriangleSurface::CTriangleSurface()
{
}
//-----------------------------------------------------------------

CTriangleSurface::~CTriangleSurface()
{
}
//-----------------------------------------------------------------

void CTriangleSurface::AddVertex(CVertex* _v, CTexVertex* _tv)
{
    if (mVertices.size() == 3)
    {
        return;
    }

    mVertices.push_back(_v);
    mTexVertices.push_back(_tv);
}
//---------------------------------------------------------------------------

void CTriangleSurface::AddTriVertex(CVertex* _v1, CVertex* _v2, CVertex* _v3, CTexVertex* _tv1, CTexVertex* _tv2, CTexVertex* _tv3)
{
    if (!_v1 || !_v2 || !_v3)
    {
        return;
    }

    mVertices.clear();
    mVertices.push_back(_v1);
    mVertices.push_back(_v2);
    mVertices.push_back(_v3);

    mTexVertices.clear();
    mTexVertices.push_back(_tv1);
    mTexVertices.push_back(_tv2);
    mTexVertices.push_back(_tv3);
}
//-----------------------------------------------------------------

void CTriangleSurface::Draw(CTex* _tex)
{
    gRenderServer.RenderTexTriangle(mVertices, mTexVertices, _tex);
}
//-----------------------------------------------------------------

#pragma package(smart_init)
