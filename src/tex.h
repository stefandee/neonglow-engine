//------------------------------------------------------------------------------
// This file is part of NeonGlow Engine.                                      //
//                                                                            //
// NeonGlow Engine - software rendering generic 3D engine                     //
// Copyright (C) 2003 Stefan Dicu                                             //
//                                                                            //
// Licensed under MIT license.                                                //
//------------------------------------------------------------------------------

// Quick and Dirty: texture holder class; only for WAL textures

#ifndef TEXH
#define TEXH

// taken from http://www.flipcode.com/tutorials/tut_q2levels.shtml
/*
struct _TWalHeader
{
char name[32];        // name of the texture
int  width;           // width (in pixels) of the largest mipmap level
int  height;          // height (in pixels) of the largest mipmap level
int  offset[4];       // byte offset of the start of each of the 4 mipmap levels
char next_name[32];   // name of the next texture in the animation
int  flags;           // ?
int  contents;        // ?
int  value;           // ?
} TWalHeader;
*/

#include <string>

typedef struct _TQ2PalEntry
{
    unsigned char mR, mG, mB;
} TQ2PalEntry;

class CTex
{
public: // c-d
    CTex();
    ~CTex();

public: // get-set
    //void SetWidth(int _v) { mWidth = _v; }
    int  GetWidth()
    {
        return mWidth;
    }

    //void SetHeight(int _v) { mHeight = _v; }
    int  GetHeight()
    {
        return mHeight;
    }

    void SetPallete(TQ2PalEntry* _v)
    {
        mPal = _v;
    };

public: // ops
    bool Load(const char* _filename, TQ2PalEntry* _pal);
    char * GetData(int _mipLevel)
    {
        return mData;
    }
    void Draw();

protected:

private:
    char* mData; // no mip-map for the moment
    int mWidth, mHeight;
    std::string mName;
    TQ2PalEntry* mPal;
};

#endif
