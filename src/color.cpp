//------------------------------------------------------------------------------
// This file is part of NeonGlow Engine.                                      //
//                                                                            //
// NeonGlow Engine - software rendering generic 3D engine                     //
// Copyright (C) 2003 Stefan Dicu                                             //
//                                                                            //
// Licensed under MIT license.                                                //
//------------------------------------------------------------------------------

#pragma hdrstop

#include "color.h"
//---------------------------------------------------------------------------

CRGB::CRGB(int _r, int _g, int _b)
{
    mColor = (_r << 16) | (_g << 8) | _b;
}
//---------------------------------------------------------------------------

#pragma package(smart_init)
