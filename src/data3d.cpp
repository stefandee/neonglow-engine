//------------------------------------------------------------------------------
// This file is part of NeonGlow Engine.                                      //
//                                                                            //
// NeonGlow Engine - software rendering generic 3D engine                     //
// Copyright (C) 2003 Stefan Dicu                                             //
//                                                                            //
// Licensed under MIT license.                                                //
//------------------------------------------------------------------------------

#include <iostream>
#include "data3d.h"
#include "rserver.h"
#include "CSurface.h"
#include "CTriangleSurface.h"

// swap shit
template <class T>
void SWAP(T& a, T& b)
{
    T c = a;
    a = b;
    b = c;
}

CVertex::CVertex(const CVertex& _v)
{
    mOriginal = _v.mOriginal;
}
//-----------------------------------------------------------------

CVertex& CVertex::operator= (const CVertex& _v)
{
    mOriginal = _v.mOriginal;

    return *this;
}
//-----------------------------------------------------------------


CObject3D::CObject3D()
{
}
//-----------------------------------------------------------------

CObject3D::~CObject3D()
{

}
//-----------------------------------------------------------------


//void CObject3D::AddSurface(CSurface* _s)
//{
//  mSurfaces.push_back(_s);
//}
void CObject3D::AddSurface(CVertex _p1, CVertex _p2, CVertex _p3, CTexVertex _tv1, CTexVertex _tv2, CTexVertex _tv3)
{
    CVertex* lV1 = 0, *lV2 = 0, *lV3 = 0;
    CTexVertex *lTV1 = 0, *lTV2 = 0, *lTV3 = 0;

    // vertices init
    for(unsigned int i = 0; i < mVertices.size(); i++)
    {
        if (mVertices[i]->GetOriginal() == _p1.GetOriginal())
        {
            lV1 = mVertices[i];
        }

        if (mVertices[i]->GetOriginal() == _p2.GetOriginal())
        {
            lV2 = mVertices[i];
        }

        if (mVertices[i]->GetOriginal() == _p3.GetOriginal())
        {
            lV3 = mVertices[i];
        }
    }

    if (!lV1)
    {
        lV1 = new CVertex();
        *lV1 = _p1;
        mVertices.push_back(lV1);
    }

    if (!lV2)
    {
        lV2 = new CVertex();
        *lV2 = _p2;
        mVertices.push_back(lV2);
    }

    if (!lV3)
    {
        lV3 = new CVertex();
        *lV3 = _p3;
        mVertices.push_back(lV3);
    }

    // texture vertices init
    for(unsigned int i = 0; i < mTexVertices.size(); i++)
    {
        if ((mTexVertices[i]->GetU() == _tv1.GetU()) && (mTexVertices[i]->GetV() == _tv1.GetV()))
        {
            lTV1 = mTexVertices[i];
        }

        if ((mTexVertices[i]->GetU() == _tv2.GetU()) && (mTexVertices[i]->GetV() == _tv2.GetV()))
        {
            lTV2 = mTexVertices[i];
        }

        if ((mTexVertices[i]->GetU() == _tv3.GetU()) && (mTexVertices[i]->GetV() == _tv3.GetV()))
        {
            lTV3 = mTexVertices[i];
        }
    }

    if (!lTV1)
    {
        lTV1 = new CTexVertex();
        *lTV1 = _tv1;
        mTexVertices.push_back(lTV1);
    }

    if (!lTV2)
    {
        lTV2 = new CTexVertex();
        *lTV2 = _tv2;
        mTexVertices.push_back(lTV2);
    }

    if (!lTV3)
    {
        lTV3 = new CTexVertex();
        *lTV3 = _tv3;
        mTexVertices.push_back(lTV3);
    }

    // now adding surfaces
    CTriangleSurface* lS = new CTriangleSurface();

    lS->AddTriVertex(lV1, lV2, lV3, lTV1, lTV2, lTV3);

    mSurfaces.push_back(lS);
}
//-----------------------------------------------------------------

void CObject3D::SetAxes(CVector3D _v)
{
}
//-----------------------------------------------------------------

void CObject3D::SetSysPoint(CPoint3D _p)
{
}
//-----------------------------------------------------------------

void CObject3D::Draw()
{
    //cout << "Object has " << mSurfaces.size() << " surfaces " << mVertices.size() << "vertices " << endl;

    for(unsigned int i = 0; i < mSurfaces.size(); i++)
    {
        (mSurfaces[i])->Draw(mTex);
    }
}
//-----------------------------------------------------------------

void CObject3D::ComputeBB()
{
    // scan through vertices, searching min and max for all axes
    if (mVertices.size() <= 0)
    {
        return;
    }

    float lMinX = mVertices[0]->GetOriginal().GetX();
    float lMaxX = mVertices[0]->GetOriginal().GetX();
    float lMinY = mVertices[0]->GetOriginal().GetY();
    float lMaxY = mVertices[0]->GetOriginal().GetY();
    float lMinZ = mVertices[0]->GetOriginal().GetZ();
    float lMaxZ = mVertices[0]->GetOriginal().GetZ();

    for(unsigned int i = 1; i < mVertices.size(); i++)
    {
        CPoint3D lP = mVertices[i]->GetOriginal();
        float lX = lP.GetX();
        float lY = lP.GetY();
        float lZ = lP.GetZ();

        // compare for X
        if (lX < lMinX)
        {
            lMinX = lX;
        }

        if (lX > lMaxX)
        {
            lMaxX = lX;
        }

        // compare for Y
        if (lY < lMinY)
        {
            lMinY = lY;
        }

        if (lY > lMaxY)
        {
            lMaxY = lY;
        }

        // compare for Z
        if (lZ < lMinZ)
        {
            lMinZ = lZ;
        }

        if (lZ > lMaxZ)
        {
            lMaxZ = lZ;
        }
    }

    mBB.SetLowBound(CSPoint3D((short int)lMinX, (short int)lMinY, (short int)lMinZ));
    mBB.SetHighBound(CSPoint3D((short int)lMaxX, (short int)lMaxY, (short int)lMaxZ));
}
//-----------------------------------------------------------------


void CBoundingBox::SetLowBound(CSPoint3D _v)
{
    mLowBound = _v;
    VerifyBounds();
}
//-----------------------------------------------------------------

void CBoundingBox::SetHighBound(CSPoint3D _v)
{
    mHighBound = _v;
    VerifyBounds();
}
//-----------------------------------------------------------------

void CBoundingBox::VerifyBounds()
{
    short int lMinX = mLowBound.GetX(), lMinY = mLowBound.GetY(), lMinZ = mLowBound.GetZ();
    short int lMaxX = mHighBound.GetX(), lMaxY = mHighBound.GetY(), lMaxZ = mHighBound.GetZ();

    if (lMinX > lMaxX)
    {
        SWAP(lMinX, lMaxX);
    }

    if (lMinY > lMaxY)
    {
        SWAP(lMinY, lMaxY);
    }

    if (lMinZ > lMaxZ)
    {
        SWAP(lMinZ, lMaxZ);
    }

    mLowBound = CSPoint3D(lMinX, lMinY, lMinZ);
    mHighBound = CSPoint3D(lMaxX, lMaxY, lMaxZ);
}
//-----------------------------------------------------------------

