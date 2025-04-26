//------------------------------------------------------------------------------
// This file is part of NeonGlow Engine.                                      //
//                                                                            //
// NeonGlow Engine - software rendering generic 3D engine                     //
// Copyright (C) 2003 Stefan Dicu                                             //
//                                                                            //
// Licensed under MIT license.                                                //
//------------------------------------------------------------------------------

#include "bspworld.h"
#include "rserver.h"
#include <iostream>

using std::vector;
using std::cout;
using std::endl;

void CSector::AddObject(CObject3D* _v)
{
    mObjects.push_back(_v);
}
//-----------------------------------------------------------------

CBspNode::CBspNode()
{
    mPlane = 0;
    mFaceIndex = -1;
    mFaceCount = 0;
}
//-----------------------------------------------------------------

void CBspNode::SetPlane(CPlane _v)
{
    if (mPlane)
    {
        delete mPlane;
        mPlane = 0;
    }

    mPlane = new CPlane(_v);
}
//-----------------------------------------------------------------

CBspNode::~CBspNode()
{
    if (mPlane)
    {
        delete mPlane;
        mPlane = 0;
    }
}
//-----------------------------------------------------------------

CClusterVisibility::CClusterVisibility()
{
    mPvsData = 0;
}
//-----------------------------------------------------------------

bool CClusterVisibility::IsClusterVisible(int _index)
{
    //int v = offset;
    int v = 0;

    //memset(cluster_visible, 0, mClusters);

    for (int c = 0; c < mClusters; v++)
    {
        if (mPvsData[v] == 0)
        {
            v++;
            c += 8 * mPvsData[v];
        }
        else
        {
            for (unsigned char bit = 1; bit != 0; bit *= 2, c++)
            {
                if ((mPvsData[v] & bit) && (_index == c))
                {
                    //cluster_visible[c] = 1;
                    return true;
                }
            }
        }
    }

    return false;
}
//-----------------------------------------------------------------

CClusterVisibility::~CClusterVisibility()
{
    if (mPvsData)
    {
        delete mPvsData;
        mPvsData = 0;
    }
}
//-----------------------------------------------------------------

CBspWorld::CBspWorld()
{
    mBase = 0;
    mCamera = 0;
    mVertices.clear();
    mBspFaces.clear();
    mFaceEdge.clear();
    mPlanes.clear();
    mBspEdges.clear();
    mBspTex.clear();
}
//-----------------------------------------------------------------

long CBspWorld::AddPlane(CPlane* _p)
{
    mPlanes.push_back(_p);

    return mPlanes.size() - 1;
}
//-----------------------------------------------------------------

long CBspWorld::AddVertex(CPoint3D* _p)
{
    mVertices.push_back(_p);

    return mVertices.size() - 1;
}
//-----------------------------------------------------------------

long CBspWorld::AddBspEdge(short int _p1, short int _p2)
{
    CBspEdge* lV = new CBspEdge(_p1, _p2);

    mBspEdges.push_back(lV);

    if (lV->GetPoint1() > (int)mVertices.size() - 1)
    {
        cout << "! warning: V1 edge index out of bounds" << endl;
    }

    if (lV->GetPoint2() > (int)mVertices.size() - 1)
    {
        cout << "! warning: edge index out of bounds" << endl;
    }

    return mBspEdges.size() - 1;
}
//-----------------------------------------------------------------

long CBspWorld::AddBspFace(CBspFace* _v)
{
    mBspFaces.push_back(_v);

    return mBspFaces.size() - 1;
}
//-----------------------------------------------------------------

long CBspWorld::AddBspTex(CBspTex* _v)
{
    mBspTex.push_back(_v);

    return mBspTex.size() - 1;
}
//-----------------------------------------------------------------

long CBspWorld::AddFaceEdge(unsigned int _v)
{
    mFaceEdge.push_back(_v);

    return mFaceEdge.size() - 1;
}
//-----------------------------------------------------------------

long CBspWorld::AddLeafFace(int _v)
{
    mLeafFace.push_back(_v);

    return mLeafFace.size() - 1;
}
//-----------------------------------------------------------------

void CBspWorld::Render()
{
    // type some data
    /*
    cout << "Bsp World Render" << endl;
    cout << ": " << mVertices.size() << " vertices " << endl;
    cout << ": " << mBspEdges.size() << " edges " << endl;
    cout << ": " << mPlanes.size() << " planes " << endl;
    cout << ": " << mFaceEdge.size() << " face edge entries " << endl;
    cout << ": " << mBspFaces.size() << " faces entries " << endl;
    cout << ": " << mBspTex.size() << " tex entries " << endl;
    */

    /*
    for(int i = 0; i < mBspTex.size(); i++)
    {
      CPoint3D lU = mBspTex[i]->GetUAxis();
      CPoint3D lV = mBspTex[i]->GetVAxis();
      float lUOff = mBspTex[i]->GetUOffset();
      float lVOff = mBspTex[i]->GetVOffset();

      cout << "TexInfo #" << i << endl;
      cout << "U: (" << lU.GetX() << ", " << lU.GetY() << ", " << lU.GetZ() << ") - " << lUOff << endl;
      cout << "V: (" << lV.GetX() << ", " << lV.GetY() << ", " << lV.GetZ() << ") - " << lVOff << endl;
      cout << "(" << mBspTex[i]->GetTex()->GetWidth() << "x" << mBspTex[i]->GetTex()->GetHeight() << ")" << endl;

      //mBspTex[i]->GetTex()->Draw();
    }
    */

    // seek the node/leaf the camera is into

    // render the node/leaf and all clusters that are visible from the current leaf/node

    // for now, just render all the faces, for test issues
    vector<CPoint3D*> lPoints;
    vector<CTexVertex*> lTexVerts;
    int lFacesRendered = 0, lFacesInCube = 0;

    // 2380
    for(int i = 0; i < (int)mBspFaces.size(); i++)
    {
        CBspFace* lFace = mBspFaces[i];
        int lEdgeIndex = lFace->GetEdgeIndex();
        CBspEdge* lEdge;
        CTex* lTex = 0;
        CBspTex* lBspTex = 0;

        if (lEdgeIndex < 0)
        {
            continue;
        }

        // only draw faces in a cube around camera - test purposes
        //if (!IsInside(lFace))
        //{
        //  continue;
        //}

        lFacesInCube++;

        // check if the face is visible
        CPlane* lPlane = mPlanes[lFace->GetPlaneIndex()];
        CVector3D lNormal = lPlane->GetNormal();
        float lVisInfo =  lNormal * mCamera->GetN();

        if (lFace->GetPlaneSide())
        {
            if (lVisInfo >= 0.0)
            {
                continue;
            }
        }
        else
        {
            if (lVisInfo <= 0.0)
            {
                continue;
            }
        }

        lFacesRendered++;

        //cout << "Face #" << i << ": tex info: " << lFace->GetTexInfo() << endl;
        if (lFace->GetTexInfo() >= 0 && lFace->GetTexInfo() < (int)mBspTex.size())
        {
            lBspTex = mBspTex[lFace->GetTexInfo()];
            lTex = lBspTex->GetTex();
        }

        if (!lTex)
        {
            cout << "CBspWorld::Render() - no tex for this face" << endl;
            continue;
        }

        // cout << "Face #" << i << ": edges " << lFace->GetEdgeCount() << " from " << lEdgeIndex << endl;
        for(int j = 0; j < lFace->GetEdgeCount(); j++)
        {
            // get the edge data in the face edge list
            //cout << "reading faceedge index " << endl;

            int lFaceEdgeIndex = (int)mFaceEdge[lEdgeIndex + j];

            CPoint3D* lP1;

            //cout << "comparing..." << endl;
            if (lFaceEdgeIndex < 0)
            {
                //cout << "a minus one " << lFaceEdgeIndex << endl;
                lEdge = mBspEdges[-lFaceEdgeIndex];

                lP1 = mVertices[lEdge->GetPoint2()];
                //lP2 = mVertices[lEdge->GetPoint1()];
            }
            else
            {
                //cout << "a plus one" << lFaceEdgeIndex << endl;
                lEdge = mBspEdges[lFaceEdgeIndex];

                lP1 = mVertices[lEdge->GetPoint1()];
                //lP2 = mVertices[lEdge->GetPoint2()];
            }

            lPoints.push_back(lP1);
            //lPoints.push_back(lP2);

            //cout << "? (" << lBspTex->ComputeU(*lP1) << "," << lBspTex->ComputeV(*lP1) << ")" << endl;

            //int u = lBspTex->ComputeU(*lP1);
            //int v = lBspTex->ComputeV(*lP1);

            lTexVerts.push_back(new CTexVertex(lBspTex->ComputeU(*lP1), lBspTex->ComputeV(*lP1)));
            //lTexVerts.push_back(new CTexVertex(lBspTex->ComputeU(*lP2), lBspTex->ComputeV(*lP2)));
        }

        // setup texverts
        if (lTex)
        {
            gRenderServer.RenderBspTexTriangle(lPoints, lTexVerts, lTex, lBspTex);
        }
        else
        {
            //gRenderServer.RenderTriangle(lPoints);
            cout << "no tex" << endl;
        }

        lPoints.clear();

        for(int i = 0; i < (int)lTexVerts.size(); i++)
        {
            delete lTexVerts[i];
        }

        lTexVerts.clear();
    }

    //cout << ": " << lFacesInCube << " faces in cube, " << lFacesRendered << " faces rendered" << endl;
    //cout << ": done rendering" << endl;
}
//-----------------------------------------------------------------

void CBspWorld::CleanBspTree(CBspArch* _v)
{
    if (_v->GetLeft())
    {
        CleanBspTree(_v->GetLeft());
        delete _v->GetLeft();
        _v->SetLeft(0);
    }

    if (_v->GetRight())
    {
        CleanBspTree(_v->GetRight());
        delete _v->GetRight();
        _v->SetRight(0);
    }
}
//-----------------------------------------------------------------

int  CBspWorld::GetEdgeIndex(int _faceEdgeIndex)
{
    return 0;
}
//-----------------------------------------------------------------

bool CBspWorld::IsInside(CBspFace* _face)
{
    CBspEdge* lEdge;
    int lEdgeIndex = _face->GetEdgeIndex(), lVertexIndex1, lVertexIndex2;
    CPoint3D lView = mCamera->GetViewPoint();

    for(int j = 0; j < _face->GetEdgeCount(); j++)
    {
        int lFaceEdgeIndex = (int)mFaceEdge[lEdgeIndex + j];

        if (lFaceEdgeIndex < 0)
        {
            lEdge = mBspEdges[-lFaceEdgeIndex];

            lVertexIndex1 = lEdge->GetPoint2();
            lVertexIndex2 = lEdge->GetPoint1();
        }
        else
        {
            lEdge = mBspEdges[lFaceEdgeIndex];

            lVertexIndex1 = lEdge->GetPoint1();
            lVertexIndex2 = lEdge->GetPoint2();
        }

        // check inside for both indexes
        CPoint3D lP1 = *mVertices[lVertexIndex1];
        CPoint3D lP2 = *mVertices[lVertexIndex2];

//    cout << "Vertices index " << lVertexIndex1 << ", " << lVertexIndex2 << endl;
//    cout << "1: " << lP1.GetX() << ", " << lP1.GetY() << ", " << lP1.GetZ() << endl;
//    cout << "2: " << lP2.GetX() << ", " << lP2.GetY() << ", " << lP2.GetZ() << endl;

        if ( (lView.GetX() - 500 < lP1.GetX() && lP1.GetX() < lView.GetX() + 500) &&
                (lView.GetY() - 500 < lP1.GetY() && lP1.GetY() < lView.GetY() + 500) &&
                (lView.GetZ() - 500 < lP1.GetZ() && lP1.GetZ() < lView.GetZ() + 500))
        {
        }
        else
        {
            return false;
        }

        if ( (lView.GetX() - 500 < lP2.GetX() && lP2.GetX() < lView.GetX() + 500) &&
                (lView.GetY() - 500 < lP2.GetY() && lP2.GetY() < lView.GetY() + 500) &&
                (lView.GetZ() - 500 < lP2.GetZ() && lP2.GetZ() < lView.GetZ() + 500))
        {
        }
        else
        {
            return false;
        }
    }

    return true;
}
//-----------------------------------------------------------------

CBspWorld::~CBspWorld()
{
    // free allocated objects
    for(int i = 0; i < (int)mVertices.size(); i++)
    {
        delete mVertices[i];
    }

    for(int i = 0; i < (int)mBspEdges.size(); i++)
    {
        delete mBspEdges[i];
    }

    for(int i = 0; i < (int)mPlanes.size(); i++)
    {
        delete mPlanes[i];
    }

    for(int i = 0; i < (int)mBspTex.size(); i++)
    {
        delete mBspTex[i];
    }

    for(int i = 0; i < (int)mBspFaces.size(); i++)
    {
        delete mBspFaces[i];
    }

    mVertices.clear();

    // delete bsp nodes/leaves
    if (mBase)
    {
        CleanBspTree(mBase);
        delete mBase;
    }
}
//-----------------------------------------------------------------


