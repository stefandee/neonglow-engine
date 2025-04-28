//------------------------------------------------------------------------------
// This file is part of NeonGlow Engine.                                      //
//                                                                            //
// NeonGlow Engine - software rendering generic 3D engine                     //
// Copyright (C) 2003 Stefan Dicu                                             //
//                                                                            //
// Licensed under MIT license.                                                //
//------------------------------------------------------------------------------

// Quick and Dirty: Bsp junk file; I don't bother creating files for every
// class, although in a nice project a separate folder should be made to hold
// these files w/classes.

#ifndef WORLDH
#define WORLDH

#include "data3d.h"
#include "camera.h"

class CSector
{
public: // c-d
    CSector() {};
    ~CSector() {};

public: // ops
    void AddObject(CObject3D* _v);

private:
    std::vector<CObject3D*> mObjects;
    std::vector<CSurface*>  mSurfaces;
};

class CBspArch
{
public:

    virtual CBspArch* GetLeft()
    {
        return mLeft;
    };
    virtual CBspArch* GetRight()
    {
        return mRight;
    };

    virtual void SetLeft(CBspArch* _v)
    {
        mLeft = _v;
    }
    virtual void SetRight(CBspArch* _v)
    {
        mRight = _v;
    }

    virtual const CBoundingBox& GetBB()
    {
        return mBB;
    };
    virtual void SetBB(CBoundingBox _v)
    {
        mBB = _v;
    };

public:
    CBspArch() : mLeft(0), mRight(0) {}
    virtual ~CBspArch() {}

private:
    CBspArch* mLeft, *mRight;
    CBoundingBox mBB;

    CBspArch(const CBspArch&);
    CBspArch& operator = (const CBspArch&);
};

class CBspNode : public CBspArch
{
public: // c-d
    CBspNode();
    ~CBspNode();

public: // ops

public: // get-set
    void SetFaceIndex(short int _v)
    {
        mFaceIndex = _v;
    }
    short int GetFaceIndex()
    {
        return mFaceIndex;
    }

    void SetFaceCount(short int _v)
    {
        mFaceCount = _v;
    };
    short int GetFaceCount()
    {
        return mFaceCount;
    }

    void SetPlane(CPlane* _v)
    {
        mPlane = _v;
    }
    void SetPlane(CPlane _v);
    CPlane* GetPlane()
    {
        return mPlane;
    }

protected:
private:
    CPlane* mPlane;  // pointer in the bsp world planes
    short int mFaceIndex, mFaceCount;  // these faces are used for collision detection
};

class CBspLeaf : public CBspArch
{
public: // c-d
    CBspLeaf();
    ~CBspLeaf();

public: // get-set
    short int GetCluster()
    {
        return mCluster;
    }
    void SetCluster(int _v)
    {
        mCluster = _v;
    }

    void SetFaceIndex(short int _v)
    {
        mFaceIndex = _v;
    }
    short int GetFaceIndex()
    {
        return mFaceIndex;
    }

    void SetFaceCount(short int _v)
    {
        mFaceCount = _v;
    };
    short int GetFaceCount()
    {
        return mFaceCount;
    }

protected:

private:
    short int mCluster; // index in the PVS array, for visibility
    short int mFaceIndex, mFaceCount;  // these faces are to be rendered
};

class CBspTex
{
public: // c-d
    CBspTex()
    {
        mTex = 0;
    }
    ~CBspTex() {}

public: // get-set
    CPoint3D GetUAxis()
    {
        return mUAxis;
    }
    void SetUAxis(CPoint3D _v)
    {
        mUAxis = _v;
    }

    CPoint3D GetVAxis()
    {
        return mVAxis;
    }
    void SetVAxis(CPoint3D _v)
    {
        mVAxis = _v;
    }

    float GetUOffset()
    {
        return mUOffset;
    }
    void SetUOffset(float _v)
    {
        mUOffset = _v;
    }

    float GetVOffset()
    {
        return mVOffset;
    }
    void SetVOffset(float _v)
    {
        mVOffset = _v;
    }

    void SetTex(CTex* _v)
    {
        mTex = _v;
    }
    CTex* GetTex()
    {
        return mTex;
    }

public: // ops
    float ComputeU(CPoint3D& _p)
    {
        return (_p.GetX() * mUAxis.GetX() + _p.GetY() * mUAxis.GetY() + _p.GetZ() * mUAxis.GetZ() + mUOffset) / mTex->GetWidth();
    }
    float ComputeV(CPoint3D& _p)
    {
        return (_p.GetX() * mVAxis.GetX() + _p.GetY() * mVAxis.GetY() + _p.GetZ() * mVAxis.GetZ() + mVOffset) / mTex->GetHeight();
    }

protected:

private:
    CPoint3D mUAxis, mVAxis;
    float mUOffset, mVOffset;

    unsigned int mFlags, mValue;

    CTex* mTex;
};

class CBspFace
{
public: // c-d
    CBspFace()
    {
        mTexInfo = -1;
    };
    ~CBspFace() {};

public: // get-set
    void SetTexInfo(short int _v)
    {
        mTexInfo = _v;
    };
    short int GetTexInfo()
    {
        return mTexInfo;
    }

    void SetEdgeCount(short int _v)
    {
        mEdgeCount = _v;
    };
    short int GetEdgeCount()
    {
        return mEdgeCount;
    }

    void SetEdgeIndex(int _v)
    {
        mEdgeIndex = _v;
    };
    short int GetEdgeIndex()
    {
        return mEdgeIndex;
    }

    void SetPlaneIndex(int _v)
    {
        mPlaneIndex = _v;
    };
    short int GetPlaneIndex()
    {
        return mPlaneIndex;
    }

    void SetPlaneSide(int _v)
    {
        mPlaneSide = _v;
    };
    short int GetPlaneSide()
    {
        return mPlaneSide;
    }

protected:

private:
    short int mTexInfo; // index in the tex info list
    int mEdgeIndex;
    short int mEdgeCount;
    short int mPlaneIndex, mPlaneSide;
};

class CBspEdge
{
public: // c-d
    CBspEdge() : mPoint1(0), mPoint2(0) {};
    CBspEdge(unsigned short int _v1, unsigned short int _v2) : mPoint1(_v1), mPoint2(_v2) {};
    ~CBspEdge() {};

public: // get-set
    void SetPoint1(unsigned short int _v)
    {
        mPoint1 = _v;
    }
    unsigned short int  GetPoint1()
    {
        return mPoint1;
    }

    void SetPoint2(unsigned short int _v)
    {
        mPoint2 = _v;
    }
    unsigned short int  GetPoint2()
    {
        return mPoint2;
    }

private:
    unsigned short int mPoint1, mPoint2; // indexes in world vertices
};

class CClusterVisibility
{
public: // c-d
    CClusterVisibility();
    ~CClusterVisibility();

public: // get-set
    char* GetPvsData()
    {
        return mPvsData;
    }
    void SetPvsData(char* _v)
    {
        mPvsData = _v;
    }

    void SetClusters(int _v)
    {
        mClusters = _v;
    }
    int GetClusters()
    {
        return mClusters;
    }

public: // ops
    bool IsClusterVisible(int _index);

protected:

private:
    char* mPvsData;
    int   mClusters;
};

class CBspWorld
{
public: // c-d
    CBspWorld();
    ~CBspWorld();

public: // get-set
    void SetBaseNode(CBspArch* _v)
    {
        mBase = _v;
    }
    CBspArch* GetBaseNode()
    {
        return mBase;
    }

    void SetCamera(CCamera* _v)
    {
        mCamera = _v;
    }
    CCamera* GetCamera()
    {
        return mCamera;
    }

public: // ops
    long AddPlane(CPlane* _p);
    long AddVertex(CPoint3D* _p);
    long AddBspEdge(short int _p1, short int _p2);
    long AddBspFace(CBspFace* _v);
    long AddBspTex(CBspTex* _v);
    long AddFaceEdge(unsigned int _v);
    long AddLeafFace(int _v);

    void Render();  // render as q2
    void Render2(); // render as q3

private:
    std::vector<CPoint3D*>        mVertices;
    std::vector<CBspEdge*>        mBspEdges;
    std::vector<CPlane*>          mPlanes;
    std::vector<CBspTex*>         mBspTex;
    std::vector<CBspFace*>        mBspFaces;
    std::vector<int>     mFaceEdge;
    std::vector<int>              mLeafFace;

    CBspArch*   mBase;
    CCamera*    mCamera;

    void CleanBspTree(CBspArch* _v);
    int  GetEdgeIndex(int _faceEdgeIndex);
    bool IsInside(CBspFace* _face);
};

#endif


