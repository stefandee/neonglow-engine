//------------------------------------------------------------------------------
// This file is part of NeonGlow Engine.                                      //
//                                                                            //
// NeonGlow Engine - software rendering generic 3D engine                     //
// Copyright (C) 2003 Stefan Dicu                                             //
//                                                                            //
// Licensed under MIT license.                                                //
//------------------------------------------------------------------------------

// Quick and Dirty: a camera class
#ifndef CAMERAH
#define CAMERAH

#include "data3d.h"
//#include "objlib.h"

/*
class CCamera
{
  public: // c-d
    CCamera() {};
    CCamera(CPoint3D _p, CVector3D _v, float _d);
    ~CCamera();

  public: // get-set
    CPoint3D GetViewPoint();
    void     SetViewPoint(CPoint3D _p);

    CVector3D GetDir();
    void     SetDir(CVector3D _v);

    float    GetDist();
    void     SetDist(float _d);

  public:// ops
    void LookAt(CPoint3D _v);
    virtual void Draw();
    CViewFrustrum ComputeViewFrustrum();
    CMatrix4 ComputeMatrix();

  protected:
    void ComputePlane();

  private:
    CPoint3D  mViewPoint;
    CVector3D mDirection;
    float     mDist;
    CPlane    mPlane;
    CObject3D* mAvatar;
};
*/

class CCamera
{
public: // c-d
    CCamera() {};
    CCamera(CPoint3D _view, CPoint3D _focus, CPoint3D _up, float _d);
    ~CCamera();

public: // get-set
    CPoint3D GetViewPoint();
    void     SetViewPoint(CPoint3D _p);

    CPoint3D GetFocusPoint();
    void     SetFocusPoint(CPoint3D _v);

    CPoint3D GetUpPoint();
    void     SetUpPoint(CPoint3D _v);

    float    GetDist();
    void     SetDist(float _d);

    CVector3D GetN()
    {
        return mN;
    };
    CVector3D GetU()
    {
        return mU;
    };
    CVector3D GetV()
    {
        return mV;
    };

public:// ops
    virtual void Draw();
    CMatrix4 ComputeMatrix();

protected:
    void ComputeAxis();

private:
    CPoint3D  mViewPoint, mFocusPoint, mUpPoint;
    CVector3D mN, mU, mV;
    float     mDist;
    //CPlane    mPlane;
    CObject3D* mAvatar;
};

#endif
