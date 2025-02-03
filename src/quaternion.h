//------------------------------------------------------------------------------
// This file is part of NeonGlow Engine.                                      //
//                                                                            //
// NeonGlow Engine - software rendering generic 3D engine                     //
// Copyright (C) 2003 Stefan Dicu                                             //
//                                                                            //
// Licensed under MIT license.                                                //
//------------------------------------------------------------------------------
//---------------------------------------------------------------------------

#ifndef quaternionH
#define quaternionH
//---------------------------------------------------------------------------

class CQuaternion
{
public: // c-d
    CQuaternion();
    CQuaternion(double _x, double _y, double _z, double _w);

public: // ops
    void Normalize();
    void Conjugate();
    CQuaternion operator *(const CQuaternion& _q); // multiplying quaternions

private:
    double x, y, z, w;
};

#endif
