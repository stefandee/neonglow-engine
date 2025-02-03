//------------------------------------------------------------------------------
// This file is part of NeonGlow Engine.                                      //
//                                                                            //
// NeonGlow Engine - software rendering generic 3D engine                     //
// Copyright (C) 2003 Stefan Dicu                                             //
//                                                                            //
// Licensed under MIT license.                                                //
//------------------------------------------------------------------------------

#ifndef CTriangleSurfaceH
#define CTriangleSurfaceH
//---------------------------------------------------------------------------

#include "CSurface.h"

class CTriangleSurface : public CSurface
{
public: // c-d
    CTriangleSurface();
    ~CTriangleSurface();

public: // ops
    virtual void AddVertex(CVertex* _v, CTexVertex* _tv);
    virtual void AddTriVertex(CVertex* _v1, CVertex* _v2, CVertex* _v3, CTexVertex* _tv1, CTexVertex* _tv2, CTexVertex* _tv3);
    virtual void Draw(CTex* _tex);
};
//---------------------------------------------------------------------------

#endif
