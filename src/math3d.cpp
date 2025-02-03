//------------------------------------------------------------------------------
// This file is part of NeonGlow Engine.                                      //
//                                                                            //
// NeonGlow Engine - software rendering generic 3D engine                     //
// Copyright (C) 2003 Stefan Dicu                                             //
//                                                                            //
// Licensed under MIT license.                                                //
//------------------------------------------------------------------------------

#include <math.h>
#include "math3d.h"
#include "geom.h"

CVector2D::CVector2D(float _x, float _y)
{
    mX = _x;
    mY = _y;
    mModule = sqrt(mX * mX + mY * mY);
}
//-----------------------------------------------------------------

void CVector2D::Normalize()
{
    if (mModule == 0)
    {
        return;
    }

    mX = mX / mModule;
    mY = mY / mModule;

    mModule = 1.0;
}
//-----------------------------------------------------------------

CVector2D CVector2D::operator +(const CVector2D& _v)
{
    return CVector2D(mX + _v.mX, mY + _v.mY);
}
//-----------------------------------------------------------------

CVector2D CVector2D::operator -(const CVector2D& _v)
{
    return CVector2D(mX - _v.mX, mY - _v.mY);
}
//-----------------------------------------------------------------

float CVector2D::operator *(const CVector2D& _v)
{
    return mX * _v.mX + mY * _v.mY;
}
//-----------------------------------------------------------------

CVector3D::CVector3D(float _x, float _y, float _z)
{
    mX = _x;
    mY = _y;
    mZ = _z;
    mModule = sqrt(mX * mX + mY * mY + mZ * mZ);
};

void CVector3D::Normalize()
{
    if (mModule == 0)
    {
        return;
    }

    mX = mX / mModule;
    mY = mY / mModule;
    mZ = mZ / mModule;

    mModule = 1.0;
}
//-----------------------------------------------------------------

CVector3D CVector3D::operator +(const CVector3D& _v)
{
    return CVector3D(mX + _v.mX, mY + _v.mY, mZ + _v.mZ);
}
//-----------------------------------------------------------------

CVector3D CVector3D::operator -(const CVector3D& _v)
{
    return CVector3D(mX - _v.mX, mY - _v.mY, mZ - _v.mZ);
}
//-----------------------------------------------------------------

float CVector3D::operator *(const CVector3D& _v)
{
    return mX * _v.mX + mY * _v.mY + mZ * _v.mZ;
}
//-----------------------------------------------------------------

CVector3D CVector3D::VProduct(const CVector3D& _v)
{
    return CVector3D(mY * _v.mZ - mZ * _v.mY, mZ * _v.mX - mX * _v.mZ, mX * _v.mY - mY * _v.mX);
}
//-----------------------------------------------------------------

CMatrix3::CMatrix3()
{
    Null();
}
//-----------------------------------------------------------------

CMatrix3 CMatrix3::operator +(const CMatrix3& _v)
{
    CMatrix3 lResult;

    for(int i =0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            lResult.mV[i][j] = mV[i][j] + _v.mV[i][j];
        }
    }

    return lResult;
}
//-----------------------------------------------------------------

CMatrix3 CMatrix3::operator -(const CMatrix3& _v)
{
    CMatrix3 lResult;

    for(int i =0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            lResult.mV[i][j] = mV[i][j] - _v.mV[i][j];
        }
    }

    return lResult;
}
//-----------------------------------------------------------------

CMatrix3 CMatrix3::operator *(const CMatrix3& _v)
{
    CMatrix3 lResult;

    for(int i =0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            for(int k = 0; k < 3; k++)
            {
                lResult.mV[i][j] += mV[i][k] * _v.mV[k][j];
            }
        }
    }

    return lResult;
}
//-----------------------------------------------------------------

float*   CMatrix3::operator [](const int i)
{
    return mV[i];

    // throw exception if i, j are invalid
}
//-----------------------------------------------------------------

void CMatrix3::Null()
{
    for(int i =0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            mV[i][j] = 0;
        }
    }
}
//-----------------------------------------------------------------

void CMatrix3::Identity()
{
    Null();
    mV[0][0] = 1.0;
    mV[1][1] = 1.0;
    mV[2][2] = 1.0;
}
//-----------------------------------------------------------------

void CMatrix3::ScalarMultiply(float _v)
{
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            mV[i][j] = mV[i][j] * _v;
        }
    }
}
//-----------------------------------------------------------------

void CMatrix3::Rotation(float _angle)
{
    Identity();
    mV[0][0] = cos(_angle);
    mV[0][1] = sin(_angle);
    mV[1][0] = -sin(_angle);
    mV[1][1] = cos(_angle);
}
//-----------------------------------------------------------------

CMatrix4::CMatrix4()
{
    Null();
}
//-----------------------------------------------------------------

CMatrix4 CMatrix4::operator +(const CMatrix4& _v)
{
    CMatrix4 lResult;

    for(int i =0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            lResult.mV[i][j] = mV[i][j] + _v.mV[i][j];
        }
    }

    return lResult;
}
//-----------------------------------------------------------------

CMatrix4 CMatrix4::operator -(const CMatrix4& _v)
{
    CMatrix4 lResult;

    for(int i =0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            lResult.mV[i][j] = mV[i][j] - _v.mV[i][j];
        }
    }

    return lResult;
}
//-----------------------------------------------------------------

CMatrix4 CMatrix4::operator *(const CMatrix4& _v)
{
    CMatrix4 lResult;

    lResult.Null();

    for(int i =0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            for(int k = 0; k < 4; k++)
            {
                lResult.mV[i][j] += _v.mV[i][k] * mV[k][j];
            }
        }
    }

    return lResult;
}
//-----------------------------------------------------------------

float*   CMatrix4::operator [](const int i)
{
    return mV[i];

    // throw exception if i, j are invalid
}
//-----------------------------------------------------------------

void CMatrix4::Null()
{
    for(int i =0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            mV[i][j] = 0;
        }
    }
}
//-----------------------------------------------------------------

void CMatrix4::Identity()
{
    Null();
    mV[0][0] = 1.0;
    mV[1][1] = 1.0;
    mV[2][2] = 1.0;
    mV[3][3] = 1.0;
}
//-----------------------------------------------------------------

void CMatrix4::ScalarMultiply(float _v)
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            mV[i][j] = mV[i][j] * _v;
        }
    }
}
//-----------------------------------------------------------------

void CMatrix4::XRotation(float _angle)
{
    Identity();
    mV[1][1] = cos(_angle);
    mV[1][2] = sin(_angle);
    mV[2][1] = -sin(_angle);
    mV[2][2] = cos(_angle);
}
//-----------------------------------------------------------------

void CMatrix4::YRotation(float _angle)
{
    Identity();
    mV[0][0] = cos(_angle);
    mV[0][2] = -sin(_angle);
    mV[2][0] = sin(_angle);
    mV[2][2] = cos(_angle);
}
//-----------------------------------------------------------------

void CMatrix4::ZRotation(float _angle)
{
    Identity();
    mV[0][0] = cos(_angle);
    mV[0][1] = sin(_angle);
    mV[1][0] = -sin(_angle);
    mV[1][1] = cos(_angle);
}
//-----------------------------------------------------------------

void CMatrix4::Translation(float _xt, float _yt, float _zt)
{
    Identity();
    mV[0][3] = _xt;
    mV[1][3] = _yt;
    mV[2][3] = _zt;
}
//-----------------------------------------------------------------

CPoint3D CMatrix4::Transform(CPoint3D _p)
{
    float lX, lY, lZ;

    lX = mV[0][0] * _p.GetX() + mV[0][1] * _p.GetY() + mV[0][2] * _p.GetZ() + mV[0][3];
    lY = mV[1][0] * _p.GetX() + mV[1][1] * _p.GetY() + mV[1][2] * _p.GetZ() + mV[1][3];
    lZ = mV[2][0] * _p.GetX() + mV[2][1] * _p.GetY() + mV[2][2] * _p.GetZ() + mV[2][3];

    return CPoint3D(lX, lY, lZ);
}
//-----------------------------------------------------------------

