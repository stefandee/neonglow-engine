//------------------------------------------------------------------------------
// This file is part of NeonGlow Engine.                                      //
//                                                                            //
// NeonGlow Engine - software rendering generic 3D engine                     //
// Copyright (C) 2003 Stefan Dicu                                             //
//                                                                            //
// Licensed under MIT license.                                                //
//------------------------------------------------------------------------------

// Quick and Dirty: texture manager

#ifndef TEXMGRH
#define TEXMGRH

#include "tex.h"
#include <vector>

class CTexMgr
{
public: // c-d
    CTexMgr();
    ~CTexMgr();

public: // ops
    CTex* GetTex(int _index);
    bool RemoveTex(int _index);
    bool AddTex(const char* _filename);
    int Count()
    {
        return mTextures.size();
    }

protected:
private:
    std::vector<CTex*> mTextures;
    TQ2PalEntry mPal[256];

    void LoadPal(const char* _filename);
};

#endif
