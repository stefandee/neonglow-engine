//------------------------------------------------------------------------------
// This file is part of NeonGlow Engine.                                      //
//                                                                            //
// NeonGlow Engine - software rendering generic 3D engine                     //
// Copyright (C) 2003 Stefan Dicu                                             //
//                                                                            //
// Licensed under MIT license.                                                //
//------------------------------------------------------------------------------


#include "bitutils.h"
#include <iostream>
#include <string.h>

int Swap32(int _v)
{
    /*
    int a = _v & 0x000000FF;
    int b = (_v & 0x0000FF00) >> 8;
    int c = (_v & 0x00FF0000) >> 16;
    int d = (_v & 0xFF000000) >> 24;

    return (a << 24) | (b << 16) | (c << 8) | d;
    */

    return _v;
}

unsigned int SwapU32(unsigned int _v)
{
    /*
      unsigned int a = _v & 0x000000FF;
      unsigned int b = (_v & 0x0000FF00) >> 8;
      unsigned int c = (_v & 0x00FF0000) >> 16;
      unsigned int d = (_v & 0xFF000000) >> 24;

      return (a << 24) | (b << 16) | (c << 8) | d;
    */
    return _v;
}

float SwapFloat(float _v)
{
    /*
      int lInt;

      memcpy(&lInt, &_v, 4);
      lInt = Swap32(lInt);
      memcpy(&_v, &lInt, 4);

      return _v;
    */
    return _v;
}

short int Swap16(short int _v)
{
    /*
      short int a = _v & 0x00FF;
      short int b = (_v & 0xFF00) >> 8;

      return (a << 8) | b;
    */
    return _v;
}

unsigned short int SwapU16(unsigned short int _v)
{
    /*
      unsigned short int a = _v & 0x00FF;
      unsigned short int b = (_v & 0xFF00) >> 8;

      return (a << 8) | b;
    */
    return _v;
}

