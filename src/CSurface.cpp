//------------------------------------------------------------------------------
// This file is part of NeonGlow Engine.                                      //
//                                                                            //
// NeonGlow Engine - software rendering generic 3D engine                     //
// Copyright (C) 2003 Stefan Dicu                                             //
//                                                                            //
// Licensed under MIT license.                                                //
//------------------------------------------------------------------------------

#pragma hdrstop

#include "CSurface.h"
#include "data3d.h"
//---------------------------------------------------------------------------

CSurface::CSurface()
{
}
//-----------------------------------------------------------------

CSurface::~CSurface()
{
}
//-----------------------------------------------------------------

void CSurface::AddVertex(CVertex* _v, CTexVertex* _tv)
{
    mVertices.push_back(_v);
    mTexVertices.push_back(_tv);
}
//-----------------------------------------------------------------

#pragma package(smart_init)
