//------------------------------------------------------------------------------
// This file is part of NeonGlow Engine.                                      //
//                                                                            //
// NeonGlow Engine - software rendering generic 3D engine                     //
// Copyright (C) 2003 Stefan Dicu                                             //
//                                                                            //
// Licensed under MIT license.                                                //
//------------------------------------------------------------------------------

// Quick and dirty - data3d.h
// header for defining 3d data - vertex, polygon, objects, sectors, world

#ifndef DATA3DH
#define DATA3DH

#include "geom.h"
#include "math3d.h"
#include <vector>

#include "tex.h"

class CSurface;

class CViewFrustrum
{
public: // c-d
    CViewFrustrum() {};
    ~CViewFrustrum() {};

public: // get-set
    const CPlane& GetTop();
    void SetTop(CPlane _v)
    {
        mTop = _v;
    };

    const CPlane& GetBottom();
    void SetBottom(CPlane _v)
    {
        mBottom = _v;
    };

    const CPlane& GetLeft();
    void SetLeft(CPlane _v)
    {
        mLeft = _v;
    };

    const CPlane& GetRight();
    void SetRight(CPlane _v)
    {
        mRight = _v;
    };

    const CPlane& GetFar();
    void SetFar(CPlane _v)
    {
        mFar = _v;
    };

    const CPlane& GetClose();
    void SetClose(CPlane _v)
    {
        mClose = _v;
    };


private:
    CPlane mTop, mBottom, mLeft, mRight, mFar, mClose;

};

class CBoundingBox
{
public: // c-d
    CBoundingBox() {};
    ~CBoundingBox() {};

public: // get-set
    const CSPoint3D& GetLowBound()
    {
        return mLowBound;
    };
    void SetLowBound(CSPoint3D _v);

    const CSPoint3D& GetHighBound()
    {
        return mHighBound;
    };
    void SetHighBound(CSPoint3D _v) ;

protected:
    virtual void VerifyBounds();

private:
    CSPoint3D mLowBound;
    CSPoint3D mHighBound;
};

/*
class CTexVertex
{
  public: // c-d
    CTexVertex() : mV(0), mU(0) {};
    CTexVertex(short int _u, short int _v) { mU = _u; mV = _v; }
    ~CTexVertex() {};

  public: // get-set
    short int GetU() { return mU; }
    void SetU(short int _v) { mU = _v; }

    short int GetV() { return mV; }
    void SetV(short int _v) { mV = _v; }

  protected:
  private:
    short int mU, mV;
};*/

class CTexVertex
{
public: // c-d
    CTexVertex() : mU(0), mV(0) {};
    CTexVertex(float _u, float _v)
    {
        mU = _u;
        mV = _v;
    }
    ~CTexVertex() {};

public: // get-set
    float GetU()
    {
        return mU;
    }
    void SetU(float _v)
    {
        mU = _v;
    }

    float GetV()
    {
        return mV;
    }
    void SetV(float _v)
    {
        mV = _v;
    }

protected:
private:
    float mU, mV;
};

class CVertex
{
public:
    CVertex() {};
    ~CVertex() {};
    CVertex(const CVertex& _v);
    CVertex& operator = (const CVertex& _v);

public: //get-set
    void SetValue(const CPoint3D& _v)
    {
        mPoint = _v;
    };
    CPoint3D GetValue()
    {
        return mPoint;
    };

    void SetOriginal(const CPoint3D& _v)
    {
        mOriginal = _v;
    };
    CPoint3D GetOriginal()
    {
        return mOriginal;
    };

private:
    CPoint3D mPoint, mOriginal;
    bool     mTransformed; // ???
    int      mU, mV; // texture coordinates
};


// advanced rendering classes
// class CBezierSurface

class CObject3D
{
public: // c-d
    CObject3D();
    ~CObject3D();

public: // ops
    void AddSurface(CVertex _p1, CVertex _p2, CVertex _p3, CTexVertex _tv1, CTexVertex _tv2, CTexVertex _tv3);
    //void AddSurface(CVertex _p1, CVertex _p2, CVertex _p3);
    void SetAxes(CVector3D _v);
    void SetSysPoint(CPoint3D _p);
    void SetTexture(CTex* _tex)
    {
        mTex = _tex;
    }
    void Draw();
    void ComputeBB();
    CBoundingBox GetBB()
    {
        return mBB;
    }

protected:
    std::vector<CObject3D*> mChildren;
    std::vector<CSurface*>  mSurfaces;
    std::vector<CVertex*>   mVertices;
    std::vector<CTexVertex*> mTexVertices;
    CPoint3D  mSystem; // the origin of its coordinate system
    CVector3D mAxes;   // the coordinate system axes
    CTex* mTex;        // texture for the whole model
    CBoundingBox mBB;  // improves rendering speed

private:
};

#endif

