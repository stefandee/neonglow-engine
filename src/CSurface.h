//------------------------------------------------------------------------------
// This file is part of NeonGlow Engine.                                      //
//                                                                            //
// NeonGlow Engine - software rendering generic 3D engine                     //
// Copyright (C) 2003 Stefan Dicu                                             //
//                                                                            //
// Licensed under MIT license.                                                //
//------------------------------------------------------------------------------

#ifndef CSurfaceH
#define CSurfaceH

#include <vector>

class CVertex;
class CTexVertex;
class CTex;

#include "geom.h"

class CSurface
{
public: // c-d
    CSurface();
    ~CSurface();

public: // ops
    virtual void AddVertex(CVertex* _v, CTexVertex* _tv);
    virtual void Draw(CTex* _tex) {};

protected:
    std::vector<CVertex*> mVertices;
    std::vector<CTexVertex*> mTexVertices;
    CPlane           mPlane;
};
//---------------------------------------------------------------------------

#endif
