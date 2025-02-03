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

#include "md2load.h"
#include "bitutils.h"

CObject3D* CMd2Loader::Load(const char* filename)
{
    // reading the header and print out the info
    fstream lFile(filename, fstream::in | fstream::out | fstream::binary);

    if (!lFile.is_open())
    {
        cout << "Error opening file " << filename << endl;
        return NULL;
    }

    lFile.read ((char*)&mHeader.magic, sizeof(int));
    lFile.read ((char*)&mHeader.version, sizeof(int));
    lFile.read ((char*)&mHeader.skinWidth, sizeof(int));
    lFile.read ((char*)&mHeader.skinHeight, sizeof(int));
    lFile.read ((char*)&mHeader.frameSize, sizeof(int));
    lFile.read ((char*)&mHeader.numSkins, sizeof(int));
    lFile.read ((char*)&mHeader.numVertices, sizeof(int));
    lFile.read ((char*)&mHeader.numTexCoords, sizeof(int));
    lFile.read ((char*)&mHeader.numTriangles, sizeof(int));
    lFile.read ((char*)&mHeader.numGlCommands, sizeof(int));
    lFile.read ((char*)&mHeader.numFrames, sizeof(int));
    lFile.read ((char*)&mHeader.offsetSkins, sizeof(int));
    lFile.read ((char*)&mHeader.offsetTexCoords, sizeof(int));
    lFile.read ((char*)&mHeader.offsetTriangles, sizeof(int));
    lFile.read ((char*)&mHeader.offsetFrames, sizeof(int));
    lFile.read ((char*)&mHeader.offsetGlCommands, sizeof(int));
    lFile.read ((char*)&mHeader.offsetEnd, sizeof(int));

    mHeader.magic = Swap32(mHeader.magic);
    mHeader.version = Swap32(mHeader.version);
    mHeader.skinWidth = Swap32(mHeader.skinWidth);
    mHeader.skinHeight = Swap32(mHeader.skinHeight);
    mHeader.frameSize = Swap32(mHeader.frameSize);
    mHeader.numSkins = Swap32(mHeader.numSkins);
    mHeader.numVertices = Swap32(mHeader.numVertices);
    mHeader.numTexCoords = Swap32(mHeader.numTexCoords);
    mHeader.numTriangles = Swap32(mHeader.numTriangles);
    mHeader.numGlCommands = Swap32(mHeader.numGlCommands);
    mHeader.numFrames = Swap32(mHeader.numFrames);
    mHeader.offsetSkins = Swap32(mHeader.offsetSkins);
    mHeader.offsetTexCoords = Swap32(mHeader.offsetTexCoords);
    mHeader.offsetTriangles = Swap32(mHeader.offsetTriangles);
    mHeader.offsetFrames = Swap32(mHeader.offsetFrames);
    mHeader.offsetGlCommands = Swap32(mHeader.offsetGlCommands);
    mHeader.offsetEnd = Swap32(mHeader.offsetEnd);

    // we are going to read the frame 0 for now
    lFile.seekg(mHeader.offsetFrames + 0 * mHeader.frameSize, ios::beg);

    // read frame misc data
    lFile.read( (char*)&mFrame.scale, 3 * sizeof(float));
    lFile.read( (char*)&mFrame.translate, 3 * sizeof(float));
    lFile.read( (char*)&mFrame.name, 16);

    // data correction
    for(int i = 0; i < 3; i++)
    {
        mFrame.scale[i] = SwapFloat(mFrame.scale[i]);
        mFrame.translate[i] = SwapFloat(mFrame.translate[i]);
    }

    // now read vertices - remark: vertices are bytes and are scaled and translated using the values above
    TMd2TriangleVertex lMd2Vertex[4096];
    CVertex    lVertex[4096];

    //lVertex = new CVertex[mHeader.numVertices];

    for(int i = 0; i < mHeader.numVertices; i++)
    {
        lFile.read((char*)&lMd2Vertex[i].vertex, 3);
        lFile.read((char*)&lMd2Vertex[i].lightNormalIndex, 1);

        float lX = lMd2Vertex[i].vertex[0] * mFrame.scale[0] + mFrame.translate[0];
        float lY = lMd2Vertex[i].vertex[1] * mFrame.scale[1] + mFrame.translate[1];
        float lZ = lMd2Vertex[i].vertex[2] * mFrame.scale[2] + mFrame.translate[2];

        lVertex[i].SetOriginal(CPoint3D(lX, lY, lZ));
        //PrintVertexInfo(lVertex[i], mFrame);
    }

    // now read texture coordinated (tex vertex)
    CTexVertex    lTex[4096];
    TMd2TexVertex lTexVertex;

    lFile.seekg(mHeader.offsetTexCoords, ios::beg);

    for(int i = 0; i < mHeader.numTexCoords; i++)
    {
        lFile.read((char*)&lTexVertex.u, sizeof(short int));
        lFile.read((char*)&lTexVertex.v, sizeof(short int));

        lTexVertex.u = Swap16(lTexVertex.u);
        lTexVertex.v = Swap16(lTexVertex.v);

        lTex[i].SetU(lTexVertex.u);
        lTex[i].SetV(lTexVertex.v);
    }

    // alloc 3D object
    cout << "Reading tri data" << endl;
    mObject3D = new CObject3D();

    // now read triangle data
    lFile.seekg(mHeader.offsetTriangles, ios::beg);

    for(int i =0; i < mHeader.numTriangles; i++)
    {
        lFile.read((char*)&mTri.vertexIndices,  3 * sizeof(short));
        lFile.read((char*)&mTri.textureIndices, 3 * sizeof(short));

        for(int  j = 0; j < 3; j++)
        {
            mTri.vertexIndices[j] = Swap16(mTri.vertexIndices[j]);
            mTri.textureIndices[j] = Swap16(mTri.textureIndices[j]);
        }

        mObject3D->AddSurface(
            lVertex[mTri.vertexIndices[0]], lVertex[mTri.vertexIndices[1]], lVertex[mTri.vertexIndices[2]],
            lTex[mTri.textureIndices[0]], lTex[mTri.textureIndices[1]], lTex[mTri.textureIndices[2]]
        );
    }

    // info out!
    PrintHeaderInfo();
    PrintFrameInfo(mFrame);

    mObject3D->ComputeBB();
    CBoundingBox lBB = mObject3D->GetBB();

    CSPoint3D lP1 = lBB.GetLowBound();
    CSPoint3D lP2 = lBB.GetHighBound();

    cout << ":bounding box is (" << lP1.GetX() << ", " << lP1.GetY() << ", " << lP1.GetZ() << ") -";
    cout << " (" << lP2.GetX() << ", " << lP2.GetY() << ", " << lP2.GetZ() << ")" << endl;

    return mObject3D;
}
//-----------------------------------------------------------------

void CMd2Loader::PrintVertexInfo(TMd2TriangleVertex& _v, TMd2Frame& _f)
{
    cout << "Vertex: " << _v.vertex[0] * _f.scale[0] + _f.translate[0] << "," << _v.vertex[1] * _f.scale[1] + _f.translate[1] << "," << _v.vertex[2] * _f.scale[2] + _f.translate[2] << endl;
}
//-----------------------------------------------------------------

void CMd2Loader::PrintFrameInfo(TMd2Frame _v)
{
    cout << "Scale values: " << _v.scale[0] << " " << _v.scale[1] << " " << _v.scale[2] << endl;
    cout << "Translate values: " << _v.translate[0] << " " << _v.translate[1] << " " << _v.translate[2] << endl;
    cout << "Frame name: " << _v.name << endl;
}
//-----------------------------------------------------------------

void CMd2Loader::PrintHeaderInfo()
{
    cout << sizeof(short);
    cout << "File magic number: " << mHeader.magic << endl;
    cout << "Version: " << mHeader.version << endl;
    cout << "Skin width: " << mHeader.skinWidth << endl;
    cout << "Skin height: " << mHeader.skinHeight << endl;
    cout << "Frame size: " << mHeader.frameSize << endl;
    cout << "Skins number: " << mHeader.numSkins << endl;
    cout << "Vertices number: " << mHeader.numVertices << endl;
    cout << "TexCoords number: " << mHeader.numTexCoords << endl;
    cout << "Triangles number: " << mHeader.numTriangles << endl;
    cout << "GlCommands number: " << mHeader.numGlCommands << endl;
    cout << "Frames number: " << mHeader.numFrames << endl;
    cout << "TexCoords offset: " << mHeader.offsetTexCoords << endl;
    cout << "Triangles offset: " << mHeader.offsetTriangles << endl;
    cout << "GlCommands offset: " << mHeader.offsetGlCommands << endl;
    cout << "Frames offset: " << mHeader.offsetFrames << endl;
    cout << "End offset: " << mHeader.offsetEnd << endl;
}
//-----------------------------------------------------------------

