//------------------------------------------------------------------------------
// This file is part of NeonGlow Engine.                                      //
//                                                                            //
// NeonGlow Engine - software rendering generic 3D engine                     //
// Copyright (C) 2003 Stefan Dicu                                             //
//                                                                            //
// Licensed under MIT license.                                                //
//------------------------------------------------------------------------------

#ifndef Q3BSPLOADH
#define Q3BSPLOADH

#include "data3d.h"
#include "bspworld.h"
#include "texmgr.h"
#include <fstream>

const long Q3_BSP_MAGIC = ((long)'P' << 24) | ((long)'S' << 16) | ((long)'B' << 8) | (long)'I';

const long Q3_LUMP_ENTITIES     = 0; // Stores player/object positions, etc...
const long Q3_LUMP_TEXTURES     = 1; // Stores texture information
const long Q3_LUMP_PLANES       = 2; // Stores the splitting planes
const long Q3_LUMP_NODES        = 3; // Stores the BSP nodes
const long Q3_LUMP_LEAFS        = 4; // Stores the leafs of the nodes
const long Q3_LUMP_LEAFFACES    = 5; // Stores the leaf's indices into the faces
const long Q3_LUMP_LEAFBRUSHES  = 6; // Stores the leaf's indices into the brushes
const long Q3_LUMP_MODELS       = 7; // Stores the info of world models
const long Q3_LUMP_BRUSHES      = 8; // Stores the brushes info (for collision)
const long Q3_LUMP_BRUSHSIDES   = 9; // Stores the brush surfaces info
const long Q3_LUMP_VERTICES     = 10;// Stores the level vertices
const long Q3_LUMP_MESHVERTS    = 11;// Stores the model vertices offsets
const long Q3_LUMP_SHADERS      = 12;// Stores the shader files (blending, anims..)
const long Q3_LUMP_FACES        = 13;// Stores the faces for the level
const long Q3_LUMP_LIGHTMAPS    = 14;// Stores the lightmaps for the level
const long Q3_LUMP_LIGHTVOLUMES = 15;// Stores extra world lighting information
const long Q3_LUMP_VISDATA      = 16;// Stores PVS and cluster info (visibility)

// a q3bsp lump
typedef struct
{
    long mOffset;
    long mLength;
} TQ3BspLump;

// the q3bsp header
typedef struct
{
    long     mMagic;
    long     mVersion;
    TQ3BspLump mLumps[19];
} TQ3BspHeader;

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
class CQ3BspLoad
{
public: // c-d
    CQ3BspLoad();
    ~CQ3BspLoad();

public: // ops
    CBspWorld* Load(const char* _filename, CTexMgr* _texMgr);

protected:

private:
    TQ3BspHeader mHdr;
    CBspWorld* mWorld;
    std::fstream    mFile;
    CTexMgr*   mTexMgr;

    void ReadVertices();
    //void ReadEdges();
    void ReadPlanes();
    //void ReadFaceEdge();
    void ReadTex();
    void ReadFaces();
    void ReadPvs();
    void ReadLeafFace();
    void ReadNodes();
    void ReadLeaves();
};

#endif

