//------------------------------------------------------------------------------
// This file is part of NeonGlow Engine.                                      //
//                                                                            //
// NeonGlow Engine - software rendering generic 3D engine                     //
// Copyright (C) 2003 Stefan Dicu                                             //
//                                                                            //
// Licensed under MIT license.                                                //
//------------------------------------------------------------------------------

#include <math.h>
#include "geom.h"

bool CPoint3D::operator == (const CPoint3D& _v)
{
    return ((_v.mX == mX) && (_v.mY == mY) && (_v.mZ == mZ));
}
//-----------------------------------------------------------------

float CPoint3D::GetDistance(CPoint3D _p)
{
    return sqrt((mX - _p.mX) * (mX - _p.mX) + (mY - _p.mY) * (mY - _p.mY) + (mZ - _p.mZ) * (mZ - _p.mZ));
}
//-----------------------------------------------------------------

bool CSPoint3D::operator == (const CSPoint3D& _v)
{
    return ((_v.mX == mX) && (_v.mY == mY) && (_v.mZ == mZ));
}
//-----------------------------------------------------------------


