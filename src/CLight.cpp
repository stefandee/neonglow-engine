//------------------------------------------------------------------------------
// This file is part of NeonGlow Engine.                                      //
//                                                                            //
// NeonGlow Engine - software rendering generic 3D engine                     //
// Copyright (C) 2003 Stefan Dicu                                             //
//                                                                            //
// Licensed under MIT license.                                                //
//------------------------------------------------------------------------------

#pragma hdrstop

#include "CLight.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

CLight::CLight()
{
    mColor               = CRGB(255, 255, 255);
    mHasIntensityFalloff = false;
    mMaxRange            = 1.0;
    mIntensity           = 100;
    //mLensOptions;
    mPosition            = CPoint3D(0., 0., 0.);
    mDir                 = CVector3D(1.0, 0., 0.);
}
//---------------------------------------------------------------------------

void CLight::SetMaxRange(float _v)
{
    if (_v < 0.)
    {
        return;
    }

    mMaxRange = _v;
}
//---------------------------------------------------------------------------

void CLight::SetIntensity(int _v)
{
    if (_v < 0 || _v > 1000)
    {
        return;
    }

    mIntensity = _v;
}
//---------------------------------------------------------------------------

void CLight::Draw()
{
}
//---------------------------------------------------------------------------

