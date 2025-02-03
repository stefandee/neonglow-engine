//------------------------------------------------------------------------------
// This file is part of NeonGlow Engine.                                      //
//                                                                            //
// NeonGlow Engine - software rendering generic 3D engine                     //
// Copyright (C) 2003 Stefan Dicu                                             //
//                                                                            //
// Licensed under MIT license.                                                //
//------------------------------------------------------------------------------

// Quick and dirty - geom.h
// holding classes related to geometry: 2d/3d vertices, planes and operations involving geometric entities

#ifndef GEOMH
#define GEOMH

#include "math3d.h"

class CPoint2D
{
public: // c-d
    CPoint2D()
    {
        mX = 0.0;
        mY = 0.0;
    };
    CPoint2D(float _x, float _y) : mX(_x), mY(_y) {};
    CPoint2D(double _x, double _y) : mX(_x), mY(_y) {};
    ~CPoint2D() {};

public: // get-set
    float GetX()
    {
        return mX;
    };
    float GetY()
    {
        return mY;
    };

protected:

private:
    float mX, mY;
};

class CPoint3D
{
public: // c-d
    CPoint3D()
    {
        mX = 0.0;
        mY = 0.0;
        mZ = 0.0;
    };
    //CPoint3D(double _x, double _y, double _z) : mX(_x), mY(_y), mZ(_z) {};
    CPoint3D(float _x, float _y, float _z) : mX(_x), mY(_y), mZ(_z) {};
    ~CPoint3D() {};

public: // get-set
    float GetX()
    {
        return mX;
    };
    float GetY()
    {
        return mY;
    };
    float GetZ()
    {
        return mZ;
    };
    float GetDistance(CPoint3D _p);

public: // ops
    bool operator == (const CPoint3D& _v);

protected:

private:
    float mX, mY, mZ;
};

class CSPoint3D
{
public: // c-d
    CSPoint3D()
    {
        mX = 0;
        mY = 0;
        mZ = 0;
    };
    //CPoint3D(double _x, double _y, double _z) : mX(_x), mY(_y), mZ(_z) {};
    CSPoint3D(short int _x, short int _y, short int _z) : mX(_x), mY(_y), mZ(_z) {};
    ~CSPoint3D() {};

public: // get-set
    short int GetX()
    {
        return mX;
    };
    short int GetY()
    {
        return mY;
    };
    short int GetZ()
    {
        return mZ;
    };

public: // ops
    bool operator == (const CSPoint3D& _v);

protected:

private:
    short int mX, mY, mZ;
};


class CPlane
{
public:
    CPlane(float _a, float _b, float _c, float _d) : mA(_a), mB(_b), mC(_c), mD(_d) {};
    CPlane()
    {
        mA = mB = mC = mD = 0.0;
    };
    CPlane(const CPlane& _v)
    {
        mA = _v.mA;
        mB = _v.mB;
        mC = _v.mC;
        mD = _v.mD;
    };
    ~CPlane() {};

public:
    CVector3D GetNormal()
    {
        return CVector3D(mA, mB, mC);
    };
    float GetDist()
    {
        return mD;
    };

protected:
    float mA, mB, mC, mD;
};

#endif

