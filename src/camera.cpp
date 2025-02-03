//------------------------------------------------------------------------------
// This file is part of NeonGlow Engine.                                      //
//                                                                            //
// NeonGlow Engine - software rendering generic 3D engine                     //
// Copyright (C) 2003 Stefan Dicu                                             //
//                                                                            //
// Licensed under MIT license.                                                //
//------------------------------------------------------------------------------

#include "camera.h"
#include "geom.h"
#include "data3d.h"
#include "math3d.h"
#include <iostream>

/*
CCamera::CCamera(CPoint3D _p, CVector3D _v, float _d)
{
  mViewPoint = _p;
  mDirection = _v;
  mDist      = _d;
  mDirection.Normalize();
  ComputePlane();
}
//-----------------------------------------------------------------

CCamera::~CCamera()
{
  mViewPoint = CPoint3D(0, 0, 0);
  mDirection = CVector3D(1., 0, 0);
  mDist      = 10.0;
  mDirection.Normalize();
  ComputePlane();
}
//-----------------------------------------------------------------

CPoint3D CCamera::GetViewPoint()
{
  return mViewPoint;
}
//-----------------------------------------------------------------

void CCamera::SetViewPoint(CPoint3D _p)
{
  mViewPoint = _p;
  ComputePlane();
}
//-----------------------------------------------------------------

CVector3D CCamera::GetDir()
{
  return mDirection;
}
//-----------------------------------------------------------------

void CCamera::SetDir(CVector3D _v)
{
  mDirection = _v;
  mDirection.Normalize();
  ComputePlane();
}
//-----------------------------------------------------------------

float CCamera::GetDist()
{
  return mDist;
}
//-----------------------------------------------------------------

void CCamera::SetDist(float _d)
{
  mDist = _d;
  ComputePlane();
}
//-----------------------------------------------------------------

void CCamera::LookAt(CPoint3D _v)
{
  CVector3D lNewDir(_v.GetX() - mViewPoint.GetX(), _v.GetY() - mViewPoint.GetY(), _v.GetZ() - mViewPoint.GetZ());

  lNewDir.Normalize();

  mDirection = lNewDir;
  ComputePlane();
}
//-----------------------------------------------------------------

CMatrix4 CCamera::ComputeMatrix()
{
  // will compute the transformation matrix (aligns the direction with y axis)
  CMatrix4 lR1;
  CMatrix4 lR2;
  CMatrix4 lT;

  lT.Translation(-mViewPoint.GetX(), -mViewPoint.GetY(), -mViewPoint.GetZ());

  float sin1 = mDirection.GetY() / sqrt(mDirection.GetX() * mDirection.GetX() + mDirection.GetY() * mDirection.GetY());
  float cos1 = mDirection.GetX() / sqrt(mDirection.GetX() * mDirection.GetX() + mDirection.GetY() * mDirection.GetY());

  lR1.Identity();
  lR1[0][0] = cos1;
  lR1[0][1] = -sin1;
  lR1[1][0] = cos1;
  lR1[1][1] = sin1;

  float sin2 = mDirection.GetZ();
  float cos2 = sqrt(mDirection.GetX() * mDirection.GetX() + mDirection.GetY() * mDirection.GetY());

  lR2.Identity();
  lR2[1][1] = cos2;
  lR2[1][2] = sin2;
  lR2[2][1] = -sin2;
  lR2[2][2] = cos2;

//  CMatrix4 lInterm = lR1 * lT;

  CPoint3D lP = CPoint3D(mDirection.GetX(), mDirection.GetY(), mDirection.GetZ());

  CMatrix4 lR;

  lR = lR1 * lR2;

  cout << "original vector is: (" << lP.GetX() << ", " << lP.GetY() << ", " << lP.GetZ() << ")" << endl;

  lP = lR.Transform(lP);

  cout << "vector is: (" << lP.GetX() << ", " << lP.GetY() << ", " << lP.GetZ() << ")" << endl;

  return (lT * lR1) * lR2;
}
//-----------------------------------------------------------------

void CCamera::Draw()
{

}
//-----------------------------------------------------------------

void CCamera::ComputePlane()
{
  CPoint3D lOnPlane(mDirection.GetX() * mDist + mViewPoint.GetX(), mDirection.GetY() * mDist + mViewPoint.GetY(), mDirection.GetZ() * mDist + mViewPoint.GetZ());

  float lD = - (lOnPlane.GetX() * mDirection.GetX() + lOnPlane.GetY() * mDirection.GetY() + lOnPlane.GetZ() * mDirection.GetZ());

  mPlane = CPlane(mDirection.GetX(), mDirection.GetY(), mDirection.GetZ(), lD);
}
//-----------------------------------------------------------------

CViewFrustrum CCamera::ComputeViewFrustrum()
{
  CViewFrustrum lVF;

  return lVF;
}
//-----------------------------------------------------------------
*/

CCamera::CCamera(CPoint3D _view, CPoint3D _focus, CPoint3D _up, float _d)
{
    mViewPoint = _view;
    mFocusPoint = _focus;
    mUpPoint = _up;
    mDist = _d;

    ComputeAxis();
}
//-----------------------------------------------------------------

CCamera::~CCamera()
{
}
//-----------------------------------------------------------------

CPoint3D CCamera::GetViewPoint()
{
    return mViewPoint;
}
//-----------------------------------------------------------------

void     CCamera::SetViewPoint(CPoint3D _p)
{
    mViewPoint = _p;
    ComputeAxis();
}
//-----------------------------------------------------------------

CPoint3D CCamera::GetFocusPoint()
{
    return mFocusPoint;
}
//-----------------------------------------------------------------

void     CCamera::SetFocusPoint(CPoint3D _p)
{
    mFocusPoint = _p;
    ComputeAxis();
}
//-----------------------------------------------------------------

CPoint3D CCamera::GetUpPoint()
{
    return mUpPoint;
}
//-----------------------------------------------------------------

void     CCamera::SetUpPoint(CPoint3D _p)
{
    mUpPoint = _p;
    ComputeAxis();
}
//-----------------------------------------------------------------

float    CCamera::GetDist()
{
    return mDist;
}
//-----------------------------------------------------------------

void     CCamera::SetDist(float _d)
{
    mDist = _d;
}
//-----------------------------------------------------------------

void CCamera::Draw()
{
}
//-----------------------------------------------------------------

CMatrix4 CCamera::ComputeMatrix()
{
    CMatrix4 lR;
    CVector3D lVRP (mViewPoint.GetX(), mViewPoint.GetY(), mViewPoint.GetZ());

    lR.Identity();

    lR[0][0] = mU.GetX();
    lR[0][1] = mV.GetX();
    lR[0][2] = mN.GetX();
    lR[0][3] = lVRP * mU;

    lR[1][0] = mU.GetY();
    lR[1][1] = mV.GetY();
    lR[1][2] = mN.GetY();
    lR[1][3] = lVRP * mV;

    lR[2][0] = mU.GetZ();
    lR[2][1] = mV.GetZ();
    lR[2][2] = mN.GetZ();
    lR[2][3] = lVRP * mN;

    /*
    for(int i = 0 ; i < 4; i++)
    {
      for(int j = 0 ; j < 4; j++)
      {
        cout << lR[i][j] << " ";
      }

      cout << endl;
    }
    */

    // test the matrix
    /*
    CPoint3D lP1(15, 20, -40);
    CPoint3D lP2(123, -452, 12);
    CPoint3D lP3(-23, -56, 20);
    CVector3D lV1(lP2.GetX() - lP1.GetX(), lP2.GetY() - lP1.GetY(), lP2.GetZ() - lP1.GetZ());
    CVector3D lV2(lP3.GetX() - lP1.GetX(), lP3.GetY() - lP1.GetY(), lP3.GetZ() - lP1.GetZ());
    lV1.Normalize();
    lV2.Normalize();

    //cout << ": distance1 is " << lP1.GetDistance(lP2) << endl;
    //cout << ": distance2 is " << lP1.GetDistance(lP3) << endl;
    //cout << ": cos is " << lV1 * lV2 << endl;

    lP1 = lR.Transform(lP1);
    lP2 = lR.Transform(lP2);
    lP3 = lR.Transform(lP3);

    lV1 = CVector3D (lP2.GetX() - lP1.GetX(), lP2.GetY() - lP1.GetY(), lP2.GetZ() - lP1.GetZ());
    lV2= CVector3D(lP3.GetX() - lP1.GetX(), lP3.GetY() - lP1.GetY(), lP3.GetZ() - lP1.GetZ());
    lV1.Normalize();
    lV2.Normalize();

    //cout << ": distance1 is " << lP1.GetDistance(lP2) << endl;
    //cout << ": distance2 is " << lP1.GetDistance(lP3) << endl;
    //cout << ": cos is " << lV1 * lV2 << endl;
    */

    return lR;
}
//-----------------------------------------------------------------

void CCamera::ComputeAxis()
{
    // compute N (it's simple)
    /*
    mN = CVector3D(mFocusPoint.GetX() - mViewPoint.GetX(), mFocusPoint.GetY() - mViewPoint.GetY(), mFocusPoint.GetZ() - mViewPoint.GetZ());

    // compute V
    CVector3D lUV = CVector3D(mUpPoint.GetX() - mViewPoint.GetX(), mUpPoint.GetY() - mViewPoint.GetY(), mUpPoint.GetZ() - mViewPoint.GetZ());

    float lScalar = lUV * mN;
    //cout << "scalar " << lScalar << endl;

    if (mN.GetModule() == 0)
    {
      lScalar = 10000.;
    }
    else
    {
      lScalar = lScalar / mN.GetModule();
    }

    mN.Normalize();
    CVector3D lNUV = CVector3D(mN.GetX() * lScalar, mN.GetY() * lScalar, mN.GetZ() * lScalar);

    mV = lUV - lNUV;
    mV.Normalize();

    // compute U
    mU = mN.VProduct(mV);

    mU.Normalize();
    mN.Normalize();
    */

    //
    // Method #2
    //
    mN = CVector3D(-mFocusPoint.GetX() + mViewPoint.GetX(), -mFocusPoint.GetY() + mViewPoint.GetY(), -mFocusPoint.GetZ() + mViewPoint.GetZ());
    mN.Normalize();

    CVector3D lUV = CVector3D(mUpPoint.GetX() - mViewPoint.GetX(), mUpPoint.GetY() - mViewPoint.GetY(), mUpPoint.GetZ() - mViewPoint.GetZ());

    CVector3D lI = lUV.VProduct(mN);

    lI.Normalize();

    mU = lI;

    mV = mN.VProduct(mU);

    //cout << mN.GetX() << ", " << mN.GetY() << ", " << mN.GetZ() << endl;
    //cout << mU.GetX() << ", " << mU.GetY() << ", " << mU.GetZ() << endl;
    //cout << mV.GetX() << ", " << mV.GetY() << ", " << mV.GetZ() << endl;
    //cout << lUV.GetX() << ", " << lUV.GetY() << ", " << lUV.GetZ() << endl;
}
//-----------------------------------------------------------------
