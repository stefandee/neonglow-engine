//------------------------------------------------------------------------------
// This file is part of NeonGlow Engine.                                      //
//                                                                            //
// NeonGlow Engine - software rendering generic 3D engine                     //
// Copyright (C) 2003 Stefan Dicu                                             //
//                                                                            //
// Licensed under MIT license.                                                //
//------------------------------------------------------------------------------

#include <iostream>
#include "q2bspload.h"
#include "bitutils.h"

using std::cout;
using std::endl;
using std::fstream;
using std::string;
using std::ios;

CQ2BspLoad::CQ2BspLoad()
{
}
//-----------------------------------------------------------------

CQ2BspLoad::~CQ2BspLoad()
{
}
//-----------------------------------------------------------------

CBspWorld* CQ2BspLoad::Load(const char* _filename, CTexMgr* _texMgr)
{
    cout << "Reading Q2 BSP file " << _filename << endl;

    // reading the header and print out the info
    mFile.open(_filename, fstream::in | fstream::out | fstream::binary);

    if (!mFile.is_open())
    {
        cout << "Error opening file " << _filename << endl;
        return NULL;
    }

    // set the tex manager
    mTexMgr = _texMgr;

    // read in the header
    mFile.read((char*)&mHdr.mMagic, sizeof(long));
    mFile.read((char*)&mHdr.mVersion, sizeof(long));

    mHdr.mMagic   = Swap32(mHdr.mMagic);
    mHdr.mVersion = Swap32(mHdr.mVersion);

    // checks
    if (mHdr.mMagic != Q2_BSP_MAGIC)
    {
        cout << _filename << " is of wrong type. Load aborted." << endl;
        return 0;
    }

    if (mHdr.mVersion != 38)
    {
        cout << _filename << " is of wrong version. Load aborted." << endl;
        return 0;
    }

    cout << ": file ok, continue with lumps info reading" << endl;

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
    ReadEdges();
    ReadPlanes();
    ReadFaceEdge();
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

void CQ2BspLoad::ReadVertices()
{
    // reading vertices lump
    int lCount = mHdr.mLumps[Q2_LUMP_VERTICES].mLength / 12;

    mFile.seekg(mHdr.mLumps[Q2_LUMP_VERTICES].mOffset, ios::beg);
    cout << ": " << lCount << " vertices " << endl;

    for(int i = 0; i < lCount; i++)
    {
        float lX, lY, lZ;

        mFile.read((char*)&lX, sizeof(float));
        mFile.read((char*)&lY, sizeof(float));
        mFile.read((char*)&lZ, sizeof(float));

        lX = SwapFloat(lX);
        lY = SwapFloat(lY);
        lZ = SwapFloat(lZ);

        CPoint3D* lP = new CPoint3D(lX, lY, lZ);

        //cout << lX << "," << lY << "," << lZ << endl;

        mWorld->AddVertex(lP);
    }
}
//-----------------------------------------------------------------

void CQ2BspLoad::ReadEdges()
{
    // reading edges lump
    int lCount = mHdr.mLumps[Q2_LUMP_EDGES].mLength / 4;

    mFile.seekg(mHdr.mLumps[Q2_LUMP_EDGES].mOffset, ios::beg);
    cout << ": " << lCount << " edges " << endl;

    for(int i = 0; i < lCount; i++)
    {
        unsigned short int lV1, lV2;

        mFile.read((char*)&lV1, sizeof(short int));
        mFile.read((char*)&lV2, sizeof(short int));

        lV1 = SwapU16(lV1);
        lV2 = SwapU16(lV2);

        //CBspEdge* lEdge = new CBspEdge(lV1, lV2);

        mWorld->AddBspEdge(lV1, lV2);
    }
}
//-----------------------------------------------------------------

void CQ2BspLoad::ReadPlanes()
{
    // reading planes
    int lCount = mHdr.mLumps[Q2_LUMP_PLANES].mLength / 20;

    mFile.seekg(mHdr.mLumps[Q2_LUMP_PLANES].mOffset, ios::beg);
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

void CQ2BspLoad::ReadFaceEdge()
{
    // reading face edges
    int lCount = mHdr.mLumps[Q2_LUMP_FACEEDGETABLE].mLength / 4;

    mFile.seekg(mHdr.mLumps[Q2_LUMP_FACEEDGETABLE].mOffset, ios::beg);
    std::cout << ": " << lCount << " face edge entries " << std::endl;

    for(int i = 0; i < lCount; i++)
    {
        unsigned int lIndex;

        mFile.read((char*)&lIndex, sizeof(unsigned int));

        lIndex = SwapU32(lIndex);

        /*
        if ((int)lIndex < 0)
        {
          cout << "? -faceedge: " << -(int)lIndex << endl;
        }
        else
        {
          cout << "? faceedge: " << (int)lIndex << endl;
        }
        */

        mWorld->AddFaceEdge(lIndex);
    }
}
//-----------------------------------------------------------------

void CQ2BspLoad::ReadTex()
{
    // reading texture info
    int lCount = mHdr.mLumps[Q2_LUMP_TEXINFO].mLength / 76;

    mFile.seekg(mHdr.mLumps[Q2_LUMP_TEXINFO].mOffset, ios::beg);
    cout << ": " << lCount << " texture infos " << endl;

    for(int i = 0; i < lCount; i++)
    {
        // data
        float lUX, lUY, lUZ, lUOffset;
        float lVX, lVY, lVZ, lVOffset;

        unsigned long lFlags, lValue, lNextTex;

        char lTexName[32];

        // reading texture data
        mFile.read((char*)&lUX, sizeof(float));
        mFile.read((char*)&lUY, sizeof(float));
        mFile.read((char*)&lUZ, sizeof(float));
        mFile.read((char*)&lUOffset, sizeof(float));

        mFile.read((char*)&lVX, sizeof(float));
        mFile.read((char*)&lVY, sizeof(float));
        mFile.read((char*)&lVZ, sizeof(float));
        mFile.read((char*)&lVOffset, sizeof(float));

        mFile.read((char*)&lFlags, sizeof(long));
        mFile.read((char*)&lValue, sizeof(long));

        mFile.read(lTexName, 32);

        mFile.read((char*)&lNextTex, sizeof(long));

        // correcting data read
        /*
        lUX = SwapFloat(lUX);
        lUY = SwapFloat(lUY);
        lUZ = SwapFloat(lUZ);
        lUOffset = SwapFloat(lUOffset);
        lVX = SwapFloat(lVX);
        lVY = SwapFloat(lVY);
        lVZ = SwapFloat(lVZ);
        lVOffset = SwapFloat(lVOffset);
        */

        lNextTex = SwapU32(lNextTex);

        //cout << ":: texture \"" << lTexName << "\"" << endl;

        // create a new bsp tex
        CBspTex* lBspTex = new CBspTex();

        lBspTex->SetUAxis(CPoint3D(lUX, lUY, lUZ));
        lBspTex->SetVAxis(CPoint3D(lVX, lVY, lVZ));
        lBspTex->SetUOffset(lUOffset);
        lBspTex->SetVOffset(lVOffset);

        // cout << "Loading texture" << lTexName << endl;

        // now load the texture data
        bool lResult = mTexMgr->AddTex((string("data\\engine\\tex\\") + string(lTexName) + string(".wal")).c_str());

        //cout << "(" << lUX << "," << lUY << "," << lUZ << " - " << lUOffset << ") - (" << lVX << "," << lVY << "," << lVZ << " - " << lUOffset << ")" << endl;

        if (!lResult)
        {
            cout << ":: texture " << string(lTexName) << " does not exist" << endl;
        }
        else
        {
            CTex* lTex = mTexMgr->GetTex(mTexMgr->Count() - 1);
            //cout << ":: texture is " << mTexMgr->Count() - 1 << " " << lTex->GetWidth() << "," << lTex->GetHeight() << endl;
            lBspTex->SetTex(lTex);
        }

        mWorld->AddBspTex(lBspTex);
    }
}
//-----------------------------------------------------------------

void CQ2BspLoad::ReadFaces()
{
    // reading faces
    int lCount = mHdr.mLumps[Q2_LUMP_FACES].mLength / 20;

    mFile.seekg(mHdr.mLumps[Q2_LUMP_FACES].mOffset, ios::beg);
    // cout << ": " << lCount << " faces " << endl;

    for(int i = 0; i < lCount; i++)
    {
        unsigned short int lPlane, lPlaneSide;
        unsigned long lFirstEdge;
        unsigned short int lNumEdges;
        unsigned short int lTexInfo;
        unsigned char lLightMapStyles[4];
        unsigned long lLightMapOffset;

        // read plane data
        mFile.read((char*)&lPlane, sizeof(short int));
        mFile.read((char*)&lPlaneSide, sizeof(short int));
        mFile.read((char*)&lFirstEdge, sizeof(int));
        mFile.read((char*)&lNumEdges, sizeof(short int));
        mFile.read((char*)&lTexInfo, sizeof(short int));
        mFile.read((char*)lLightMapStyles, 4 * sizeof(char));
        mFile.read((char*)&lLightMapOffset, sizeof(int));

        // correct plane data
        lPlane = Swap16(lPlane);
        lPlaneSide = Swap16(lPlaneSide);
        lFirstEdge = Swap32(lFirstEdge);
        lNumEdges = Swap16(lNumEdges);
        lTexInfo = Swap16(lTexInfo);
        lLightMapOffset = Swap32(lLightMapOffset);

        // fill in a cplane class
        CBspFace* lBspFace = new CBspFace();

        lBspFace->SetTexInfo(lTexInfo);
        lBspFace->SetEdgeCount(lNumEdges);
        lBspFace->SetEdgeIndex(lFirstEdge);
        lBspFace->SetPlaneIndex(lPlane);
        lBspFace->SetPlaneSide(lPlaneSide);

        // add it to the world
        mWorld->AddBspFace(lBspFace);
    }
}
//-----------------------------------------------------------------

void CQ2BspLoad::ReadPvs()
{
    // reading pvs data
    mFile.seekg(mHdr.mLumps[Q2_LUMP_VISIBILITY].mOffset, ios::beg);
    cout << ": " << "visibility data" << endl;
}
//-----------------------------------------------------------------

void CQ2BspLoad::ReadLeafFace()
{
    // reading leaf face
    mFile.seekg(mHdr.mLumps[Q2_LUMP_LEAFFACETABLE].mOffset, ios::beg);
    cout << ": " << mHdr.mLumps[Q2_LUMP_LEAFFACETABLE].mLength / 2 << " leaf face entries " << endl;

}
//-----------------------------------------------------------------

void CQ2BspLoad::ReadNodes()
{
    // reading bsp nodes
    mFile.seekg(mHdr.mLumps[Q2_LUMP_NODES].mOffset, ios::beg);
    cout << ": " << mHdr.mLumps[Q2_LUMP_NODES].mLength / 28 << " nodes " << endl;

}
//-----------------------------------------------------------------

void CQ2BspLoad::ReadLeaves()
{
    // reading leaves
    mFile.seekg(mHdr.mLumps[Q2_LUMP_LEAVES].mOffset, ios::beg);
    cout << ": " << mHdr.mLumps[Q2_LUMP_LEAVES].mLength / 28 << " leaves " << endl;

}
//-----------------------------------------------------------------


