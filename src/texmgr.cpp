//------------------------------------------------------------------------------
// This file is part of NeonGlow Engine.                                      //
//                                                                            //
// NeonGlow Engine - software rendering generic 3D engine                     //
// Copyright (C) 2003 Stefan Dicu                                             //
//                                                                            //
// Licensed under MIT license.                                                //
//------------------------------------------------------------------------------

#include <iostream>
#include <fstream>
using namespace std;

#include "texmgr.h"

CTexMgr::CTexMgr()
{
    mTextures.clear();
    LoadPal("data\\engine\\q2.pal");
}
//-----------------------------------------------------------------

CTexMgr::~CTexMgr()
{
    for(unsigned int i = 0; i < mTextures.size(); i++)
    {
        delete mTextures[i];
    }

    mTextures.clear();

    //delete [] mPal;
}
//-----------------------------------------------------------------

void CTexMgr::LoadPal(const char* _filename)
{
    fstream lFile(_filename, fstream::in | fstream::out | fstream::binary);

    if (!lFile.is_open())
    {
        cout << "Error opening file " << _filename << endl;
        return;
    }

    //mPal = new TQ2PalEntry[256];

    for(int i = 0; i < 256; i++)
    {
        lFile.read((char*)(&mPal[i].mR), 1);
        lFile.read((char*)(&mPal[i].mG), 1);
        lFile.read((char*)(&mPal[i].mB), 1);
    }
}
//-----------------------------------------------------------------

CTex* CTexMgr::GetTex(int _index)
{
    if (_index >= mTextures.size())
    {
        return 0;
    }

    return mTextures[_index];
}
//-----------------------------------------------------------------

bool CTexMgr::RemoveTex(int _index)
{
    if (_index >= mTextures.size())
    {
        return false;
    }

    // not implemented yet
    // delete mTextures[_index];
}
//-----------------------------------------------------------------

bool CTexMgr::AddTex(const char* _filename)
{
    CTex* lTex;
    bool lResult;

    try
    {
        lTex = new CTex();
    }
    catch(...)
    {
        return false;
    }

    try
    {
        lResult = lTex->Load(_filename, mPal);
    }
    catch(...)
    {
        delete lTex;
        return false;
    }

    if (!lResult)
    {
        delete lTex;
        return false;
    }

    mTextures.push_back(lTex);

    return true;
}
//-----------------------------------------------------------------


