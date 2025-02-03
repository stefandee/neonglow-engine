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

#include <string.h>
#include "tex.h"
#include "bitutils.h"

CTex::CTex()
{
    mData = 0;
    mWidth = -1;
    mHeight = -1;
}
//-----------------------------------------------------------------

CTex::~CTex()
{
    if (mData)
    {
        delete [] mData;
        mData = 0;
        mWidth = -1;
        mHeight = -1;
    }
}
//-----------------------------------------------------------------

bool CTex::Load(const char* _filename, TQ2PalEntry* _pal)
{
    fstream lFile(_filename, fstream::in | fstream::out | fstream::binary);

    if (!lFile.is_open())
    {
        cout << "Error opening file " << _filename << endl;
        return false;
    }

    char lName[32];
    int  lOffset[4];

    lFile.read(lName, 32);
    mName = string(lName);

    lFile.read((char*)&mWidth, sizeof(int));
    lFile.read((char*)&mHeight, sizeof(int));
    lFile.read((char*)&lOffset[0], sizeof(int));
    lFile.read((char*)&lOffset[1], sizeof(int));
    lFile.read((char*)&lOffset[2], sizeof(int));
    lFile.read((char*)&lOffset[3], sizeof(int));

    mWidth = Swap32(mWidth);
    mHeight = Swap32(mHeight);
    lOffset[0] = Swap32(lOffset[0]);
    lOffset[1] = Swap32(lOffset[1]);
    lOffset[2] = Swap32(lOffset[2]);
    lOffset[3] = Swap32(lOffset[3]);

    //cout << "WAL: " << mName << " (" << mWidth << "x" << mHeight << ")" << endl;
    //cout << "First mipmap at: " << lOffset[0] << endl;

    lFile.seekg(lOffset[0], ios::beg);

    // de-alloc, re-alloc
    if (mData)
    {
        delete [] mData;
    }

    try
    {
        mData = new char[4 * mWidth * mHeight];
    }
    catch(...)
    {
        cout << "Cannot alloc " << 4 * mWidth * mHeight << "bytes for texture data." << endl;
        return false;
    }

    // set the palette
    mPal = _pal;

    // clean the texture
    memset(mData, 0, 4 * mWidth * mHeight);

    // slow but trustworthy
    for(int y = 0; y < mHeight; y++)
    {
        for(int x = 0; x < mWidth; x++)
        {
            unsigned char lIndexedData = 0;

            lFile.read((char*)(&lIndexedData), 1);

            long lR = mPal[lIndexedData].mR, lG = mPal[lIndexedData].mG, lB = mPal[lIndexedData].mB;

            *(((long*)mData) + y * mWidth + x) = lR | (lG << 8) | (lB << 16);
        }
    }

    return true;
}
//-----------------------------------------------------------------

void CTex::Draw()
{
    if (!mData)
    {
        return;
    }
}
//-----------------------------------------------------------------

