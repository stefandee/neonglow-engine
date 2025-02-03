//------------------------------------------------------------------------------
// This file is part of NeonGlow Engine.                                      //
//                                                                            //
// NeonGlow Engine - software rendering generic 3D engine                     //
// Copyright (C) 2003 Stefan Dicu                                             //
//                                                                            //
// Licensed under MIT license.                                                //
//------------------------------------------------------------------------------

#ifndef MD2LOADH
#define MD2LOADH

#include "data3d.h"

const int gMd2Magic   = 844121161;
const int gMd2Version = 8;

typedef struct
{
    int magic;
    int version;
    int skinWidth;
    int skinHeight;
    int frameSize;
    int numSkins;
    int numVertices;
    int numTexCoords;
    int numTriangles;
    int numGlCommands;
    int numFrames;
    int offsetSkins;
    int offsetTexCoords;
    int offsetTriangles;
    int offsetFrames;
    int offsetGlCommands;
    int offsetEnd;
} TMd2FileData;

typedef struct
{
    unsigned char vertex[3];
    unsigned char lightNormalIndex;
} TMd2TriangleVertex;

typedef struct
{
    short int u;
    short int v;
} TMd2TexVertex;

typedef struct
{
    short vertexIndices[3];
    short textureIndices[3];
} TMd2Triangle;

typedef struct
{
    short s, t;
} TMd2TextureCoordinate;

typedef struct
{
    float scale[3];
    float translate[3];
    char name[16];
    TMd2TriangleVertex vertices[1];
} TMd2Frame;

class CMd2Loader
{
public: // c-d
    CMd2Loader() {};
    ~CMd2Loader() {};

public: // ops
    CObject3D* Load(const char* filename);

protected:
private:
    TMd2FileData mHeader;
    TMd2Frame    mFrame;
    TMd2TriangleVertex mVertex;
    TMd2Triangle mTri;
    CObject3D* mObject3D;

    void PrintFrameInfo(TMd2Frame _v);
    void PrintHeaderInfo();
    void PrintVertexInfo(TMd2TriangleVertex& _v, TMd2Frame& _f);
};

#endif
