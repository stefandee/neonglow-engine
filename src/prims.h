//------------------------------------------------------------------------------
// This file is part of NeonGlow Engine.                                      //
//                                                                            //
// NeonGlow Engine - software rendering generic 3D engine                     //
// Copyright (C) 2003 Stefan Dicu                                             //
//                                                                            //
// Licensed under MIT license.                                                //
//------------------------------------------------------------------------------

#ifndef primsH
#define primsH
//---------------------------------------------------------------------------
// misc functions
void setGlobalClipRect(const int _xl, const int _yt, const int _xr, const int _yb);

// functii de pixel
void putPixelNC (char *p, long next_scan, int x, int y, long color);
void putPixel   (char *p, long next_scan, int x, int y, long color);

// functii de linie
void vLine (char *p, long next_scan, int x,  int yStart, int yEnd, long color);
void hLine (char *p, long next_scan, int xs, int xe,     int y,    long color);
//void opaqueHLine  (char *p, long next_scan, int xs, int xe, int y, long color, long opacity);
//void darkHLine    (char *p, long next_scan, int xs, int xe, int y, long darkSide);
//void checkerHLine (char *p, long next_scan, int xs, int xe, int y, long color);

//void vLine(BYTE *p, long next_scan, int x, int yStart, int yEnd, WORD color);
//void hLine(BYTE *p, long next_scan, int xs, int xe, int y, WORD color);

// functii de dreptunghiuri
//void darkRect    (char *p, long next_scan, int xstart, int ystart, int xend, int yend, long darkSide);
//void frameRect   (char *p, long next_scan, int xstart, int ystart, int xend, int yend, long color);
void fillRect    (char *p, long next_scan, int xStart, int yStart, int xEnd, int yEnd, long color);
//void selRect     (char *p, long next_scan, int xstart, int ystart, int xend, int yend, int d, long color);
//void checkerRect (char *p, long next_scan, int xStart, int yStart, int xEnd, int yEnd, long color);

//void darkRect(BYTE *p, long next_scan, int xstart, int ystart, int xend, int yend, WORD darkSide);
//void frameRect(BYTE *p, long next_scan, int xstart, int ystart, int xend, int yend, WORD color);
//void fillRect(BYTE *p, long next_scan, int xStart, int yStart, int xEnd, int yEnd, WORD color);
//void selRect(BYTE *p, long next_scan, int xstart, int ystart, int xend, int yend, int d, WORD color);
//void checkerRect(BYTE *p, long next_scan, int xstart, int ystart, int xend, int yend, WORD color);

// functii de cercuri
void Circle(char *p, long next_scan, int x, int y, int radius, long color);

// triangle
void Triangle(char *p, int next_scan, int p0x, int p0y, int p1x, int p1y, int p2x, int p2y, long color);
void GouraudTriangle(char *p, int next_scan, int p0x, int p0y, int p1x, int p1y, int p2x, int p2y, long color0, long color1, long color2);

void TexTriangle(char *p, int next_scan, int p0x, int p0y, int p1x, int p1y, int p2x, int p2y,
                 char *tex, int tnext_scan, short int t0x, short int t0y, short int t1x, short int t1y, short int t2x, short int t2y);

void ZTexTriangle(char *p, int next_scan, int p0x, int p0y, int p1x, int p1y, int p2x, int p2y,
                  char *tex, int tnext_scan, short int t0x, short int t0y, short int t1x, short int t1y, short int t2x, short int t2y,
                  char *zbuf, int znext_scan, float zb0, float zb1, float zb2,
                  int twidth, int theight);

void ZGTexTriangle(char *p, int next_scan, int p0x, int p0y, int p1x, int p1y, int p2x, int p2y,
                   char *tex, int tnext_scan, short int t0x, short int t0y, short int t1x, short int t1y, short int t2x, short int t2y,
                   char *zbuf, int znext_scan, float zb0, float zb1, float zb2,
                   int twidth, int theight,
                   long color0, long color1, long color2);

#endif


