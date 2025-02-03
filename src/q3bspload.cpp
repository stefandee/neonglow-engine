//------------------------------------------------------------------------------
// This file is part of NeonGlow Engine.                                      //
//                                                                            //
// NeonGlow Engine - software rendering generic 3D engine                     //
// Copyright (C) 2003 Stefan Dicu                                             //
//                                                                            //
// Licensed under MIT license.                                                //
//------------------------------------------------------------------------------

#include <iostream>
#include "q3bspload.h"
#include "bitutils.h"

using std::cout;
using std::endl;
using std::fstream;

CQ3BspLoad::CQ3BspLoad()
{
}
//-----------------------------------------------------------------

CQ3BspLoad::~CQ3BspLoad()
{
}
//-----------------------------------------------------------------

CBspWorld* CQ3BspLoad::Load(const char* _filename, CTexMgr* _texMgr)
{
    cout << "Reading Q3 BSP file " << _filename << endl;

    // reading the header and print out the info
    mFile.open(_filename, fstream::in | fstream::out | fstream::binary);

    if (!mFile.is_open())
    {
        cout << "Error opening file " << _filename << endl;
        return NULL;
    }

    // sset the tex manager
    mTexMgr = _texMgr;

    // read in the header
    mFile.read((char*)&mHdr.mMagic, sizeof(long));
    mFile.read((char*)&mHdr.mVersion, sizeof(long));

    mHdr.mMagic   = Swap32(mHdr.mMagic);
    mHdr.mVersion = Swap32(mHdr.mVersion);

    // checks
    if (mHdr.mMagic != Q3_BSP_MAGIC)
    {
        cout << _filename << " is of wrong type. Load aborted." << endl;
        return 0;
    }

    if (mHdr.mVersion != 0x2E)
    {
        cout << _filename << " is of wrong version. Load aborted." << endl;
        return 0;
    }

    cout << ": bsp file version " << mHdr.mVersion << ", continue with lumps info reading" << endl;

    for(int i = 0; i < 19; i++)
    {
        mFile.read((char*)&mHdr.mLumps[i].mOffset, sizeof(long));
        mFile.read((char*)&mHdr.mLumps[i].mLength, sizeof(long));

        mHdr.mLumps[i].mOffset = Swap32(mHdr.mLumps[i].mOffset);
        mHdr.mLumps[i].mLength = Swap32(mHdr.mLumps[i].mLength);
    }

    // allocate world
    mWorld = new CBspWorld();

    // read basic data from file
    ReadVertices();
    ReadPlanes();
    ReadTex();
    ReadFaces();

    // read advanced data
    ReadPvs();
    ReadLeafFace();
    ReadNodes();
    ReadLeaves();

    mFile.close();

    return mWorld;
}
//-----------------------------------------------------------------

void CQ3BspLoad::ReadVertices()
{
    // reading vertices lump
    int lCount = mHdr.mLumps[Q3_LUMP_VERTICES].mLength / 44;

    mFile.seekg(mHdr.mLumps[Q3_LUMP_VERTICES].mOffset, ios::beg);
    cout << ": " << lCount << " vertices " << endl;

    for(int i = 0; i < lCount; i++)
    {
        float lX, lY, lZ;
        float lU, lV;
        float lMapX, lMapY;
        float lNx, lNy, lNz;
        unsigned char lColor[4];

        // reading
        mFile.read((char*)&lX, sizeof(float));
        mFile.read((char*)&lY, sizeof(float));
        mFile.read((char*)&lZ, sizeof(float));

        mFile.read((char*)&lU, sizeof(float));
        mFile.read((char*)&lV, sizeof(float));

        mFile.read((char*)&lMapX, sizeof(float));
        mFile.read((char*)&lMapY, sizeof(float));

        mFile.read((char*)&lNx, sizeof(float));
        mFile.read((char*)&lNy, sizeof(float));
        mFile.read((char*)&lNz, sizeof(float));

        mFile.read(lColor, 4 * sizeof(char));

        // correction
        lX = SwapFloat(lX);
        lY = SwapFloat(lY);
        lZ = SwapFloat(lZ);

        lU = SwapFloat(lU);
        lV = SwapFloat(lV);

        lMapX = SwapFloat(lMapX);
        lMapY = SwapFloat(lMapY);

        lNx = SwapFloat(lNx);
        lNy = SwapFloat(lNy);
        lNz = SwapFloat(lNz);

        CPoint3D* lP = new CPoint3D(lX, lY, lZ);

        mWorld->AddVertex(lP);
    }
}
//-----------------------------------------------------------------

void CQ3BspLoad::ReadPlanes()
{
    // reading planes
    int lCount = mHdr.mLumps[Q3_LUMP_PLANES].mLength / 20;

    mFile.seekg(mHdr.mLumps[Q3_LUMP_PLANES].mOffset, ios::beg);
    cout << ": " << lCount << " planes " << endl;

    for(int i = 0; i < lCount; i++)
    {
        float lA, lB, lC;
        float lD;
        long  lType;

        mFile.read((char*)&lA, sizeof(float));
        mFile.read((char*)&lB, sizeof(float));
        mFile.read((char*)&lC, sizeof(float));
        mFile.read((char*)&lD, sizeof(float));
        mFile.read((char*)&lType, sizeof(long)); // this param doesnt matter

        lA = SwapFloat(lA);
        lB = SwapFloat(lB);
        lC = SwapFloat(lC);
        lD = SwapFloat(lD);

        CPlane* lPlane = new CPlane(lA, lB, lC, lD);

        mWorld->AddPlane(lPlane);
    }
}
//-----------------------------------------------------------------

void CQ3BspLoad::ReadTex()
{
    // reading texture info
    int lCount = mHdr.mLumps[Q3_LUMP_TEXTURES].mLength / 72;

    mFile.seekg(mHdr.mLumps[Q3_LUMP_TEXTURES].mOffset, ios::beg);
    cout << ": " << lCount << " texture infos " << endl;

    for(int i = 0; i < lCount; i++)
    {
        // data
        unsigned int lFlags, lContents;

        char lTexName[64];

        // reading texture data
        mFile.read(lTexName, 64);
        mFile.read((char*)&lFlags, sizeof(int));
        mFile.read((char*)&lContents, sizeof(int));

        // correcting data read
        lFlags = SwapU32(lFlags);
        lContents = SwapU32(lContents);

        // create a new bsp tex
        // CBspTex* lBspTex = new CBspTex();

        // now load the texture data
        /*
        bool lResult = mTexMgr->AddTex((string("data/textures/") + string(lTexName) + string(".wal")).c_str());

        if (!lResult)
        {
          //cout << ":: texture does not exist" << endl;
        }
        else
        {
          CTex* lTex = mTexMgr->GetTex(mTexMgr->Count() - 1);
          //cout << ":: texture is " << mTexMgr->Count() - 1 << endl;
          lBspTex->SetTex(lTex);
        }

        mWorld->AddBspTex(lBspTex);
        */
    }
}
//-----------------------------------------------------------------

void CQ3BspLoad::ReadFaces()
{
    // reading faces
    int lCount = mHdr.mLumps[Q3_LUMP_FACES].mLength / 100;

    mFile.seekg(mHdr.mLumps[Q3_LUMP_FACES].mOffset, ios::beg);
    cout << ": " << lCount << " faces " << endl;

    for(int i = 0; i < lCount; i++)
    {
        unsigned int lTexId, lEffect, lType, lVertexIndex, lVertNo, lMeshVertIndex, lMeshVertNo, lLightMapId;
        unsigned int lMapCorner[2], lMapSize[2];
        float lMapPos[3];
        float lMapBitsets[2][3];
        float lNormal[3];
        unsigned int lBPatchDim[2];

        // read plane data
        mFile.read((char*)&lTexId, sizeof(int));
        mFile.read((char*)&lEffect, sizeof(int));
        mFile.read((char*)&lType, sizeof(int));
        mFile.read((char*)&lVertexIndex, sizeof(int));
        mFile.read((char*)&lVertNo, sizeof(int));
        mFile.read((char*)&lMeshVertIndex, sizeof(int));
        mFile.read((char*)&lMeshVertNo, sizeof(int));
        mFile.read((char*)&lLightMapId, sizeof(int));
        mFile.read((char*)&lMapCorner, 2 * sizeof(int));
        mFile.read((char*)&lMapSize, 2 * sizeof(int));
        mFile.read((char*)&lMapPos, 2 * sizeof(float));
        for(int j = 0; j < 2; j++)
        {
            mFile.read((char*)&lMapBitsets[j][0], sizeof(float));
            mFile.read((char*)&lMapBitsets[j][1], sizeof(float));
            mFile.read((char*)&lMapBitsets[j][2], sizeof(float));
        }

        mFile.read((char*)&lNormal, 3 * sizeof(float));
        mFile.read((char*)&lBPatchDim, 2 * sizeof(int));

        // correct plane data

        // fill in a cplane class
        /*
        CBspFace* lBspFace = new CBspFace();

        lBspFace->SetTexInfo(lTexInfo);
        lBspFace->SetEdgeCount(lNumEdges);
        lBspFace->SetEdgeIndex(lFirstEdge);
        lBspFace->SetPlaneIndex(lPlane);
        lBspFace->SetPlaneSide(lPlaneSide);

        // add it to the world
        mWorld->AddBspFace(lBspFace);
        */
    }
}
//-----------------------------------------------------------------

void CQ3BspLoad::ReadPvs()
{
    // reading pvs data
    mFile.seekg(mHdr.mLumps[Q3_LUMP_VISDATA].mOffset, ios::beg);
    cout << ": " << "visibility data" << endl;
}
//-----------------------------------------------------------------

void CQ3BspLoad::ReadLeafFace()
{
    // reading leaf face
    mFile.seekg(mHdr.mLumps[Q3_LUMP_LEAFFACES].mOffset, ios::beg);
    cout << ": " << mHdr.mLumps[Q3_LUMP_LEAFFACES].mLength / 2 << " leaf face entries " << endl;

}
//-----------------------------------------------------------------

void CQ3BspLoad::ReadNodes()
{
    // reading bsp nodes
    mFile.seekg(mHdr.mLumps[Q3_LUMP_NODES].mOffset, ios::beg);
    cout << ": " << mHdr.mLumps[Q3_LUMP_NODES].mLength / 28 << " nodes " << endl;

}
//-----------------------------------------------------------------

void CQ3BspLoad::ReadLeaves()
{
    // reading leaves
    mFile.seekg(mHdr.mLumps[Q3_LUMP_LEAFS].mOffset, ios::beg);
    cout << ": " << mHdr.mLumps[Q3_LUMP_LEAFS].mLength / 28 << " leaves " << endl;

}
//-----------------------------------------------------------------



