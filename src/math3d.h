//------------------------------------------------------------------------------
// This file is part of NeonGlow Engine.                                      //
//                                                                            //
// NeonGlow Engine - software rendering generic 3D engine                     //
// Copyright (C) 2003 Stefan Dicu                                             //
//                                                                            //
// Licensed under MIT license.                                                //
//------------------------------------------------------------------------------

// Quick and dirty - math3d.h
// contains classes related to 3d math: vectors, matrixes maybe quaternions

#ifndef MATH3DH
#define MATH3DH

class CPoint3D;

class CVector2D
{
public: // c-d
    CVector2D(float _x, float _y);
    ~CVector2D() {};

public: // get-set
    float GetX()
    {
        return mX;
    } ;
    float GetY()
    {
        return mY;
    };
    float GetModule()
    {
        return mModule;
    };

public: // ops
    void Normalize();
    CVector2D operator +(const CVector2D& _v); // add
    CVector2D operator -(const CVector2D& _v);  // substract
    float operator *(const CVector2D& _v); // scalar product

protected:

private:
    float mX, mY, mModule;
};

class CVector3D
{
public: // c-d
    CVector3D(float _x, float _y, float _z);
    CVector3D() : mX(0), mY(0), mZ(0), mModule(0) {};
    ~CVector3D() {};

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
    float GetModule()
    {
        return mModule;
    };

public: // ops
    void Normalize();
    CVector3D operator +(const CVector3D& _v); // add
    CVector3D operator -(const CVector3D& _v);  // substract
    float operator *(const CVector3D& _v); // scalar product
    CVector3D VProduct(const CVector3D& _v);

protected:

private:
    float mX, mY, mZ, mModule;
};

class CMatrix3
{
public: // c-d
    CMatrix3();
    ~CMatrix3() {};

public: // ops
    CMatrix3 operator +(const CMatrix3& _v);
    CMatrix3 operator -(const CMatrix3& _v);
    CMatrix3 operator *(const CMatrix3& _v);
    float*   operator [](const int i);

public: // data-altering ops
    void Null();
    void Identity();
    void ScalarMultiply(float _v);
    void Rotation(float _angle);

protected:

private:
    float mV[3][3];
};

class CMatrix4
{
public: // c-d
    CMatrix4();
    ~CMatrix4() {};

public: // ops
    CMatrix4 operator +(const CMatrix4& _v);
    CMatrix4 operator -(const CMatrix4& _v);
    CMatrix4 operator *(const CMatrix4& _v);
    float*   operator [](const int i);

public: // data-altering ops
    void Null();
    void Identity();
    void ScalarMultiply(float _v);
    void XRotation(float _angle);
    void YRotation(float _angle);
    void ZRotation(float _angle);
    void Translation(float _xt, float _yt, float _zt);
    CPoint3D Transform(CPoint3D _p);

private:
    float mV[4][4];
};

#endif

