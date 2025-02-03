//------------------------------------------------------------------------------
// This file is part of NeonGlow Engine.                                      //
//                                                                            //
// NeonGlow Engine - software rendering generic 3D engine                     //
// Copyright (C) 2003 Stefan Dicu                                             //
//                                                                            //
// Licensed under MIT license.                                                //
//------------------------------------------------------------------------------

#ifndef Q2BSPLOADH
#define Q2BSPLOADH

#include "data3d.h"
#include "bspworld.h"
#include "texmgr.h"
#include <fstream>

const long Q2_BSP_MAGIC = ((long)'P' << 24) | ((long)'S' << 16) | ((long)'B' << 8) | (long)'I';

/*
0	Entities	        MAP entity text buffer
1	Planes	                Plane array
2	Vertices	        Vertex array
3	Visibility	        Compressed PVS data and directory for all clusters
4	Nodes	                Internal node array for the BSP tree
5	Texture Information	Face texture application array
6	Faces	                Face array
7	Lightmaps	        Lightmaps
8	Leaves	                Internal leaf array of the BSP tree
9	Leaf Face Table 	Index lookup table for referencing the face array from a leaf
10	Leaf Brush Table	?
11	Edges	                Edge array
12	Face Edge Table	        Index lookup table for referencing the edge array from a face
13	Models	                ?
14	Brushes	                ?
15	Brush Sides	        ?
16	Pop	                ?
17	Areas	                ?
18	Area Portals	        ?
*/

const long Q2_LUMP_ENTITIES       = 0;
const long Q2_LUMP_PLANES         = 1;
const long Q2_LUMP_VERTICES       = 2;
const long Q2_LUMP_VISIBILITY     = 3;
const long Q2_LUMP_NODES          = 4;
const long Q2_LUMP_TEXINFO        = 5;
const long Q2_LUMP_FACES          = 6;
const long Q2_LUMP_LIGHTMAPS      = 7;
const long Q2_LUMP_LEAVES         = 8;
const long Q2_LUMP_LEAFFACETABLE  = 9;
const long Q2_LUMP_LEAFBRUSHTABLE = 10;
const long Q2_LUMP_EDGES          = 11;
const long Q2_LUMP_FACEEDGETABLE  = 12;
const long Q2_LUMP_MODELS         = 13;

// a q2bsp lump
typedef struct
{
    long mOffset;
    long mLength;
} TBspLump;

// the q2bsp header
typedef struct
{
    long     mMagic;
    long     mVersion;
    TBspLump mLumps[19];
} TBspHeader;

/*
typedef struct
{
  point3f  u_axis;
  float    u_offset;

  point3f  v_axis;
  float    v_offset;

  uint32   flags;
  uint32   value;

  char     texture_name[32];

  uint32   next_texinfo;
} TBspTexInfo;
*/

//
class CQ2BspLoad
{
public: // c-d
    CQ2BspLoad();
    ~CQ2BspLoad();

public: // ops
    CBspWorld* Load(const char* _filename, CTexMgr* _texMgr);

protected:

private:
    TBspHeader mHdr;
    CBspWorld* mWorld;
    std::fstream    mFile;
    CTexMgr*   mTexMgr;

    void ReadVertices();
    void ReadEdges();
    void ReadPlanes();
    void ReadFaceEdge();
    void ReadTex();
    void ReadFaces();
    void ReadPvs();
    void ReadLeafFace();
    void ReadNodes();
    void ReadLeaves();
};

#endif
