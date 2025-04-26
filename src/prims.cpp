//------------------------------------------------------------------------------
// This file is part of NeonGlow Engine.                                      //
//                                                                            //
// NeonGlow Engine - software rendering generic 3D engine                     //
// Copyright (C) 2003 Stefan Dicu                                             //
//                                                                            //
// Licensed under MIT license.                                                //
//------------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Descriere :  primitive grafice; pot fi folosite oriunde in memorie
//              desi sint destinate lucrului cu suprafete directdraw
//---------------------------------------------------------------------------

#pragma hdrstop

#include "prims.h"
#include <iostream>
#include <algorithm>
//#include "fixed.h"

template <class T>
void SWAP(T& a, T& b)
{
    T c = a;
    a = b;
    b = c;
}

static int XLeft  = 0,
           XRight = 639,
           YTop   = 0,
           YBot   = 479;

/*--------------------------------------------------------------------------
 Description: face ce zice (sau cel putzin ase se lauda)
--------------------------------------------------------------------------*/
void setGlobalClipRect(const int _xl, const int _yt, const int _xr, const int _yb)
{
    XLeft  = _xl;
    XRight = _xr;
    YTop   = _yt;
    YBot   = _yb;
}
//---------------------------------------------------------------------------

// Descriere   : pune un pixel pe ecran
// Param       : x, y       - coordonatele
//               next_scan  - lungimea liniei de scan (pe orizontala)
//               color      - culoare, pe 16 biti
// Rezultat    : nimic
// Comentarii  : nici unul, pe moment
void putPixelNC (char *p, long next_scan, int x, int y, long color)
{
    *((long*)(p + y*next_scan + (x << 2))) = color;
}
//---------------------------------------------------------------------------


// Descriere   : pune un pixel pe ecran, facind clip la regiunea definita de
//               XLeft, XRight, YTop, YBottom
// Param       : ca mai sus
// Rezultat    : nimic
// Comentarii  : hmmm...tot nimic
void putPixel   (char *p, long next_scan, int x, int y, long color)
{
    if ((x >= XLeft) && (x <= XRight) &&
            (y >= YTop ) && (y <= YBot))
        *((long*)(p + y*next_scan + (x << 2))) = color;
}
//---------------------------------------------------------------------------

// Descriere   :  rutina de linie verticala
// Param       :  evidenti
// Rezultat    :
// Comentarii  :
void inline vLine(char *p, long next_scan, int x, int yStart, int yEnd, long color)
{
    if ((x < XLeft) || (x > XRight)) return;
    if (yStart > yEnd) SWAP(yEnd, yStart);
    if (yStart < YTop) yStart = YTop;
    if (yEnd > YBot) yEnd = YBot;

    int y = yStart * next_scan + (x << 2);

    for(int yCount = yStart; yCount <= yEnd; yCount++)
    {
        *((long*)(p + y)) = color;
        y += next_scan;
    }
}

// Descriere   : rutina de linie orizontala
// Param       : evidenti
// Rezultat    : evident
// Comentarii  : face clipping la fereastra
//               e modificata dintr'o sursa microsoftiana
void inline hLine(char *p, long next_scan, int xs, int xe, int y, long color)
{
    // nu desenam puncte - astea se fac cu putPixel :-))
    if (xe == xs) return;

    // inversam daca s'au dat invers
    if (xe < xs) SWAP(xe,xs);

    // teste de interioritate in fereastra
    if ((y  < YTop) ||   (y  > YBot)) return;
    if ((xs > XRight) || (xe < XLeft)) return;
    if (xs  < XLeft) xs = XLeft;
    if (xe  > XRight) xe = XRight;

    p += y * next_scan + (xs * 4);

    for(int i = 0; i <= xe - xs + 1; i++)
    {
        *((long*)p) = color;
        p += 4;
    }
}
//---------------------------------------------------------------------------

// Rutina pentru desenat triunghiuri
// Inputs     :
// Comentarii : trebuie facuta in ASM
//		nu face clipping pe y
void Triangle(char *p, int next_scan, int p0x, int p0y, int p1x, int p1y, int p2x, int p2y, long _color)
{
    float   d,d0;
    float   x,x0;
    int     y;

    //
    //  sort points so P0.y <= P1.y <= P2.y
    //
    if (p0y > p1y)
    {
        SWAP(p0x,p1x);
        SWAP(p0y, p1y);
    }
    if (p0y > p2y)
    {
        SWAP(p0x,p2x);
        SWAP(p0y, p2y);
    }
    if (p1y > p2y)
    {
        SWAP(p1x,p2x);
        SWAP(p1y, p2y);
    }

    //
    //  check for quick out?
    //
    if (p2y - p0y == 0)
    {
        return;
    }

    //
    //  compute "long" side walk from P0 to P2
    //
    if ((p2y - p0y)!=0) d = (float)(p2x - p0x) / (float)(p2y - p0y);
    else d = (p2x - p0x) * 100.;


    x  = p0x;
    y  = p0y;
    //p += p0y * next_scan;   // point p to correct scan.

    //
    //  do the top
    //
    if (p0y < p1y)
    {
        if ((p1y - p0y) != 0) d0 = (float)(p1x - p0x) / (float)(p1y - p0y);
        else d0 = (p1x - p0x) * 100.;

        x0 = p0x;

        //
        // check for left or right fill
        //
        if (d < d0)
        {
            while (y < p1y)
            {
                //FillSpecial(p, x, x0, c);
                hLine(p, next_scan, (int)x, (int)x0, y, _color);
                y++;
                //p  += next_scan;
                x  += d;
                x0 += d0;
            }
        }
        else
        {
            while (y < p1y)
            {
                hLine(p, next_scan, (int)x, (int)x0, y, _color);
                //FillSpecial(p, x0, x, c);
                y++;
                //p  += next_scan;
                x  += d;
                x0 += d0;
            }
        }
    }

    //
    // do the bottom.
    //

    if (p2y - p1y == 0)
    {
        return;
    }

    if ((p2y - p1y) != 0) d0 = (float)(p2x - p1x) / (float)(p2y - p1y);
    else d0 = (p2x - p1x) * 100.;

    x0 = p1x;

    //
    // check for left or right fill
    //
    if (x < x0)
    {
        while (y < p2y)
        {
            hLine(p, next_scan, (int)x, (int)x0, y, _color);
            //FillSpecial(p, x, x0, c);
            y++;
            //p  += next_scan;
            x  += d;
            x0 += d0;
        }
    }
    else
    {
        while (y < p2y)
        {
            hLine(p, next_scan, (int)x, (int)x0, y, _color);
            //FillSpecial(p, x0, x, c);
            y++;
            //p  += next_scan;
            x  += d;
            x0 += d0;
        }
    }
}
//---------------------------------------------------------------------------


// Descriere   : rutina de linie orizontala
// Param       : evidenti
// Rezultat    : evident
// Comentarii  : face clipping la fereastra
//               e modificata dintr'o sursa microsoftiana
void inline hGouraudLine(char *p, long next_scan, int xs, int xe, int y, float rs, float gs, float bs, float re, float ge, float be)
{
    // nu desenam puncte - astea se fac cu putPixel :-))
    if (xe == xs) return;

    // inversam daca s'au dat invers
    if (xe < xs)
    {
        SWAP(xe,xs);
        SWAP(re,rs);
        SWAP(ge,gs);
        SWAP(be,bs);
    }

    // teste de interioritate in fereastra
    if ((y  < YTop) ||   (y  > YBot)) return;
    if ((xs > XRight) || (xe < XLeft)) return;

    int xLength = xe - xs + 1;
    float rLength = re - rs + 1;
    float gLength = ge - gs + 1;
    float bLength = be - bs + 1;

    if (xs  < XLeft)
    {
        xs = XLeft;
        rs += -(float)xs * rLength / xLength;
        gs += -(float)xs * gLength / xLength;
        bs += -(float)xs * bLength / xLength;
    }

    if (xe  > XRight)
    {
        xe = XRight;

        re = re - (float)(xe - XRight) * rLength / xLength;
        ge = ge - (float)(xe - XRight) * rLength / xLength;
        be = be - (float)(xe - XRight) * rLength / xLength;
    }

    p += y * next_scan + (xs * 4);

    float rd = (float)(re - rs) / xLength;
    float gd = (float)(ge - gs) / xLength;
    float bd = (float)(be - bs) / xLength;

    for(int i = 0; i < xLength; i++)
    {
        rs += rd;
        gs += gd;
        bs += bd;

        *((long*)p) = ((int)rs << 16) | ((int)gs << 8) | ((int)bs);

        p += 4;
    }
}
//---------------------------------------------------------------------------

// Description : Gouraud Shaded Triangle
// Param       :
// Result      :
// Comments    :
void GouraudTriangle(char *p, int next_scan, int p0x, int p0y, int p1x, int p1y, int p2x, int p2y, long color0, long color1, long color2)
{
    float   d,d0;
    float   x,x0;
    int     y;

    //
    //  sort points so P0.y <= P1.y <= P2.y
    //
    if (p0y > p1y)
    {
        SWAP(p0x,p1x);
        SWAP(p0y, p1y);
        SWAP(color0, color1);
    }
    if (p0y > p2y)
    {
        SWAP(p0x,p2x);
        SWAP(p0y, p2y);
        SWAP(color0, color2);
    }
    if (p1y > p2y)
    {
        SWAP(p1x,p2x);
        SWAP(p1y, p2y);
        SWAP(color1, color2);
    }

    //
    //  check for quick out?
    //
    if (p2y - p0y == 0)
    {
        return;
    }

    //
    // unfold the rgb colors
    //
    float r0 = (color0 & 0xFF0000) >> 16;
    float g0 = (color0 & 0x00FF00) >> 8;
    float b0 = (color0 & 0x0000FF);

    float r1 = (color1 & 0xFF0000) >> 16;
    float g1 = (color1 & 0x00FF00) >> 8;
    float b1 = (color1 & 0x0000FF);

    float r2 = (color2 & 0xFF0000) >> 16;
    float g2 = (color2 & 0x00FF00) >> 8;
    float b2 = (color2 & 0x0000FF);

    float rd, gd, bd;
    float rd0, gd0, bd0;

    //
    //  compute "long" side walk from P0 to P2
    //
    if ((p2y - p0y)!=0)
    {
        d = (float)(p2x - p0x) / (float)(p2y - p0y);
        rd = (r2 - r0) / (float)(p2y - p0y);
        gd = (g2 - g0) / (float)(p2y - p0y);
        bd = (b2 - b0) / (float)(p2y - p0y);
    }
    else
    {
        d = (p2x - p0x) * 100.;
        rd = (r2 - r0)   * 100.;
        gd = (g2 - g0)   * 100.;
        bd = (b2 - b0)   * 100.;
    }


    x  = p0x;
    y  = p0y;
    float r = r0, g = g0, b = b0;

    //p += p0y * next_scan;   // point p to correct scan.

    //
    //  do the top
    //
    if (p0y < p1y)
    {
        if ((p1y - p0y) != 0)
        {
            d0 = (float)(p1x - p0x) / (float)(p1y - p0y);
            rd0 = (r1 - r0) / (float)(p1y - p0y);
            gd0 = (g1 - g0) / (float)(p1y - p0y);
            bd0 = (b1 - b0) / (float)(p1y - p0y);
        }
        else
        {
            d0 = (p1x - p0x) * 100.;
            rd0 = (r1 - r0) * 100.;
            gd0 = (g1 - g0) * 100.;
            bd0 = (b1 - b0) * 100.;
        }

        x0 = p0x;
        float rx = r0, gx = g0, bx = b0;

        //
        // check for left or right fill
        //
        if (d < d0)
        {
            while (y < p1y)
            {
                //FillSpecial(p, x, x0, c);
                hGouraudLine(p, next_scan, (int)x, (int)x0, y, r, g, b, rx, gx, bx);
                y++;
                //p  += next_scan;
                x  += d;
                x0 += d0;
                r += rd;
                g += gd;
                b += bd;
                rx += rd0;
                gx += gd0;
                bx += bd0;
            }
        }
        else
        {
            while (y < p1y)
            {
                hGouraudLine(p, next_scan, (int)x, (int)x0, y, r, g, b, rx, gx, bx);
                //FillSpecial(p, x0, x, c);
                y++;
                //p  += next_scan;
                x  += d;
                x0 += d0;
                r += rd;
                g += gd;
                b += bd;
                rx += rd0;
                gx += gd0;
                bx += bd0;
            }
        }
    }

    //
    // do the bottom.
    //
    if (p2y - p1y == 0)
    {
        return;
    }

    if ((p2y - p1y) != 0)
    {
        d0 = (float)(p2x - p1x) / (float)(p2y - p1y);
        rd0 = (r2 - r1) / (float)(p2y - p1y);
        gd0 = (g2 - g1) / (float)(p2y - p1y);
        bd0 = (b2 - b1) / (float)(p2y - p1y);
    }
    else
    {
        d0 = (p2x - p1x) * 100.;
        rd0 = (r2 - r1) * 100.;
        gd0 = (g2 - g1) * 100.;
        bd0 = (b2 - b1) * 100.;
    }

    x0 = p1x;
    float rx = r1, gx = g1, bx = b1;

    //
    // check for left or right fill
    //
    if (x < x0)
    {
        while (y < p2y)
        {
            hGouraudLine(p, next_scan, (int)x, (int)x0, y, r, g, b, rx, gx, bx);
            //FillSpecial(p, x, x0, c);
            y++;
            //p  += next_scan;
            x  += d;
            x0 += d0;
            r += rd;
            g += gd;
            b += bd;
            rx += rd0;
            gx += gd0;
            bx += bd0;
        }
    }
    else
    {
        while (y < p2y)
        {
            hGouraudLine(p, next_scan, (int)x, (int)x0, y, r, g, b, rx, gx, bx);
            //FillSpecial(p, x0, x, c);
            y++;
            //p  += next_scan;
            x  += d;
            x0 += d0;
            r += rd;
            g += gd;
            b += bd;
            rx += rd0;
            gx += gd0;
            bx += bd0;
        }
    }
}
//---------------------------------------------------------------------------

// Rutina de trasat linie
// Inputs     : xs,ys - coordonate de inceput
//		xe,ye - coordonate de sfirsit
//		color - culoare
// Comentarii : nu face clipping
void LineNC(char *p, long next_scan, int xs, int ys, int xe, int ye, long color)
{
    int deltax, deltay, numpixels, d, dinc1, dinc2,
        xinc1, xinc2, yinc1, yinc2,
        screeninc1, screeninc2;

    // Calculate deltax and deltay for initialisation
    deltax = abs(xe - xs);
    deltay = abs(ye - ys);

    // Initialize all vars based on which is the independent variable
    if (deltax >= deltay)
    {
        // x is independent variable
        numpixels  = deltax + 1;
        d 	   = (2 * deltay) - deltax;
        dinc1      = deltay << 1;
        dinc2 	   = (deltay - deltax) << 1;
        xinc1      = 1;
        xinc2      = 1;
        yinc1      = 0;
        yinc2 	   = 1;
    }
    else
    {
        // y is independent variable
        numpixels  = deltay + 1;
        d 	   = (2 * deltax) - deltay;
        dinc1 	   = deltax << 1;
        dinc2      = (deltax - deltay) << 1;
        xinc1      = 0;
        xinc2      = 1;
        yinc1      = 1;
        yinc2      = 1;
    }

    // Make sure x and y move in the right directions
    if (xs > xe)
    {
        xinc1 = - xinc1;
        xinc2 = - xinc2;
    }
    if (ys > ye)
    {
        yinc1 = - yinc1;
        yinc2 = - yinc2;
    }

    // Start drawing at <x1, y1>
    screeninc1 = yinc1 * next_scan + xinc1;
    screeninc2 = yinc2 * next_scan + xinc2;
    p = p + ys * next_scan + xs*4;

    // Draw the pixels
    while (numpixels-- >=0)
    {
        *p = color;
        if (d < 0)
        {
            d = d + dinc1;
            p = p + screeninc1;
        }
        else
        {
            d = d + dinc2;
            p = p + screeninc2;
        }
    }
}
//---------------------------------------------------------------------------

// Rutina de cerc - algoritmul bresenham
// inputs     : x,y - centrul cercului
//		radius - raza
// comentarii : nu e optimizat - cele 8 apeluri la putpixel
// void Circle(BYTE *p, long next_scan, int x, int y, int radius, WORD color)
void Circle(char *p, long next_scan, int x, int y, int radius, long color)
{
    int d, xi, yi;

    radius = abs(radius);
    if (radius == 0) return;
    if (radius == 1)
    {
        putPixel(p, next_scan, x, y, color);
        return;
    }

    xi = radius;
    yi = 0;
    d  = 3 - 2 * radius;

    while (xi >= yi)
    {
        putPixel(p, next_scan, x+xi, y+yi, color);
        putPixel(p, next_scan, x-xi, y+yi, color);
        putPixel(p, next_scan, x+yi, y+xi, color);
        putPixel(p, next_scan, x-yi, y+xi, color);

        putPixel(p, next_scan, x+xi, y-yi, color);
        putPixel(p, next_scan, x-xi, y-yi, color);
        putPixel(p, next_scan, x+yi, y-xi, color);
        putPixel(p, next_scan, x-yi, y-xi, color);
        if (d<0)
        {
            yi++;
            d = d + (yi << 2) + 2;
        }
        else
        {
            yi++;
            xi--;
            d = d - (xi << 2) + (yi << 2) + 2;
        }
    }
}
//---------------------------------------------------------------------------

// Descriere   : deseneaza un dreptunghi plin
// Param       : evidenti
// Rezultat    : pe ecran
// Comentarii  : nimic
//void fillRect(BYTE *p, long next_scan, int xStart, int yStart, int xEnd, int yEnd, WORD color)
void fillRect(char *p, long next_scan, int xStart, int yStart, int xEnd, int yEnd, long  color)
{
    // clipping
    if (yStart > yEnd) SWAP(yStart, yEnd);
    if (xStart > xEnd) SWAP(xStart, xEnd);

    if (xStart < XLeft) xStart = XLeft;
    if (xEnd > XRight) xEnd = XRight;
    if (yStart < YTop) yStart = YTop;
    if (yEnd > YBot) yEnd = YBot;

    for(int y = yStart; y <= yEnd; y++)
    {
        hLine(p, next_scan, xStart, xEnd, y, color);
    }
}
//---------------------------------------------------------------------------

// Descriere   : texture horizontal line
void inline hTexLine(char *p,   long next_scan,  int xs, int xe, int y,
                     char *tex, long tnext_scan, short int tus, short int tue, short int tvs, short int tve)
{
    //cout << "htexline" << endl;

    // nu desenam puncte - astea se fac cu putPixel :-))
    if (xe == xs) return;

    // inversam daca s'au dat invers
    if (xe < xs)
    {
        SWAP(xe,xs);
        SWAP(tue,tus);
        SWAP(tvs, tve);
    }

    // teste de interioritate in fereastra
    //cout << YBot << ", " << YTop << endl;
    //cout << XRight << ", " << XLeft << endl;

    if ((y  < YTop) ||   (y  > YBot)) return;
    if ((xs > XRight) || (xe < XLeft)) return;
    if (xs  < XLeft) xs = XLeft;
    if (xe  > XRight) xe = XRight;

    p += y * next_scan + (xs * 4);
    //tex += ty * tnext_scan + (txs * 4);

    float tdu = (float)(tue - tus) / (xe - xs + 1);
    float tdv = (float)(tve - tvs) / (xe - xs + 1);
    float  tu = tus, tv = tvs;

    //cout << tu << ", " << tv << endl;

    for(int i = 0; i <= xe - xs + 1; i++)
    {
        //cout << tu << ", " << tv << " - " << tnext_scan << " - " << tv * tnext_scan + tu << endl;
        *((long*)p) = *((long*)tex + (long)tv * (tnext_scan / 4) + (long)tu);
        //*((long*)p) = *((long*)(tex + (long)tv * tnext_scan + (long)tu));
        //cout << "texel dumped" << endl;

        p += 4;
        tu += tdu;
        tv += tdv;
    }

    //cout << "end htexline" << endl;
}
//---------------------------------------------------------------------------

// Draws textured triangles
void TexTriangle(char *p, int next_scan, int p0x, int p0y, int p1x, int p1y, int p2x, int p2y,
                 char *tex, int tnext_scan, short int t0x, short int t0y, short int t1x, short int t1y, short int t2x, short int t2y)
{
    float   d, d0;
    float   x, x0;
    int     y;
    float   texUD, texUD0, texVD, texVD0;
    float   u, u0, v, v0;

    //
    //  sort points so P0.y <= P1.y <= P2.y
    //
    if (p0y > p1y)
    {
        SWAP(p0x,p1x);
        SWAP(p0y, p1y);
        SWAP(t0x, t1x);
        SWAP(t0y, t1y);
    }
    if (p0y > p2y)
    {
        SWAP(p0x,p2x);
        SWAP(p0y, p2y);
        SWAP(t0x, t2x);
        SWAP(t0y, t2y);
    }
    if (p1y > p2y)
    {
        SWAP(p1x,p2x);
        SWAP(p1y, p2y);
        SWAP(t1x, t2x);
        SWAP(t1y, t2y);
    }

    //
    //  check for quick out?
    //
    /*
    if (p2y - p0y == 0)
    {
        return;
    }
    */

    //
    //  compute "long" side walk from P0 to P2
    //
    if ((p2y - p0y)!=0)
    {
        d = (float)(p2x - p0x) / (float)(p2y - p0y);
        texUD = (float)(t2x - t0x) / (float)(p2y - p0y);
        texVD = (float)(t2y - t0y) / (float)(p2y - p0y);
    }
    else
    {
        d = (p2x - p0x) * 100.;
        texUD = (float)(t2x - t0x) * 100.0;
        texVD = (float)(t2y - t0y) * 100.0;
    }


    x  = p0x;
    y  = p0y;
    u  = t0x;
    v  = t0y;
    //p += p0y * next_scan;   // point p to correct scan.

    //
    //  do the top
    //
    if (p0y < p1y)
    {
        if ((p1y - p0y) != 0)
        {
            d0 = (float)(p1x - p0x) / (float)(p1y - p0y);
            texUD0 = (float)(t1x - t0x) / (float)(p1y - p0y);
            texVD0 = (float)(t1y - t0y) / (float)(p1y - p0y);
        }
        else
        {
            d0 = (p1x - p0x) * 100.;
            texUD0 = (float)(t1x - t0x) * 100.0;
            texVD0 = (float)(t1y - t0y) * 100.0;
        }

        x0 = p0x;
        u0 = t0x;
        v0 = t0y;

        //
        // check for left or right fill
        //
        if (d < d0)
        {
            while (y < p1y)
            {
                hTexLine(p, next_scan, (int)x, (int)x0, y, tex, tnext_scan, (short int)u, (short int)u0, (short int)v, (short int)v0);
                y++;
                //p  += next_scan;
                x  += d;
                x0 += d0;
                u0 += texUD0;
                v0 += texVD0;
                u += texUD;
                v += texVD;
            }
        }
        else
        {
            while (y < p1y)
            {
                hTexLine(p, next_scan, (int)x, (int)x0, y, tex, tnext_scan, (short int)u, (short int)u0, (short int)v, (short int)v0);
                y++;
                //p  += next_scan;
                x  += d;
                x0 += d0;
                u0 += texUD0;
                v0 += texVD0;
                u += texUD;
                v += texVD;
            }
        }
    }

    //
    // do the bottom.
    //

    /*
    if (p2y - p1y == 0)
    {
        return;
    }
    */

    if ((p2y - p1y) != 0)
    {
        d0 = (float)(p2x - p1x) / (float)(p2y - p1y);
        texUD0 = (float)(t2x - t1x) / (float)(p2y - p1y);
        texVD0 = (float)(t2y - t1y) / (float)(p2y - p1y);
    }
    else
    {
        d0 = (p2x - p1x) * 100.;
        texUD0 = (float)(t2x - t1x) * 100.;
        texVD0 = (float)(t2y - t1y) * 100.;
    }

    x0 = p1x;
    u0 = t1x;
    v0 = t1y;

    //
    // check for left or right fill
    //
    if (d < d0)
    {
        while (y < p2y)
        {
            hTexLine(p, next_scan, (int)x, (int)x0, y, tex, tnext_scan, (short int)u, (short int)u0, (short int)v, (short int)v0);
            y++;
            x  += d;
            x0 += d0;
            u0 += texUD0;
            v0 += texVD0;
            u += texUD;
            v += texVD;
        }
    }
    else
    {
        while (y < p2y)
        {
            hTexLine(p, next_scan, (int)x, (int)x0, y, tex, tnext_scan, (short int)u, (short int)u0, (short int)v, (short int)v0);
            y++;
            x  += d;
            x0 += d0;
            u0 += texUD0;
            v0 += texVD0;
            u += texUD;
            v += texVD;
        }
    }
}
//---------------------------------------------------------------------------

// Descriere   : z buffer texture horizontal line
void inline hZTexLine(char *p,   long next_scan,  int xs, int xe, int y,
                      char *tex, long tnext_scan, short int tus, short int tue, short int tvs, short int tve,
                      char *zbuf, long znext_scan, float zs, float ze)
{
    //cout << "htexline" << endl;

    // nu desenam puncte - astea se fac cu putPixel :-))
    if (xe == xs) return;

    // inversam daca s'au dat invers
    if (xe < xs)
    {
        SWAP(xe,xs);
        SWAP(tue,tus);
        SWAP(tvs, tve);
        SWAP(zs, ze);
    }

    // teste de interioritate in fereastra
    //cout << YBot << ", " << YTop << endl;
    //cout << XRight << ", " << XLeft << endl;

    if ((y  < YTop) ||   (y  > YBot)) return;
    if ((xs > XRight) || (xe < XLeft)) return;

    int tuLength = tue - tus + 1;
    int tvLength = tve - tvs + 1;
    int xLength = (xe - xs) + 1;

    if (xs  < XLeft)
    {
        // update the starting texture coordinate also
        tus += (float)(XLeft - xs) * tuLength / xLength;
        tvs += (float)(XLeft - xs) * tvLength / xLength;

        xs = XLeft;
    }

    if (xe  > XRight)
    {
        // update the end texture coordinate also
        tue = tue - (float)(xe - XRight) * tuLength / xLength;
        tve = tve - (float)(xe - XRight) * tvLength / xLength;

        xe = XRight;
    }

    xLength = (xe - xs) + 1;

    p += y * next_scan + (xs * 4);
    zbuf = zbuf + y * znext_scan + (xs * 2);
    //tex += ty * tnext_scan + (txs * 4);

    float tdu = (float)(tue - tus) / xLength;
    float tdv = (float)(tve - tvs) / xLength;
    float  tu = tus, tv = tvs;
    float  tz = zs, tdz = (float)(ze - zs) / xLength;

    //cout << "htex" << endl;
    //cout << zs << ", " << ze << endl;
    //cout << znext_scan << " ";

    //if (y > 239)
    //{
    //  cout << y << " :!";
    //}
    //else
    //{
    //  cout << y << " :";
    //}

    for(int i = 0; i < xLength; i++)
    {
        //cout << tu << ", " << tv << " - " << tnext_scan << " - " << tv * tnext_scan + tu << endl;

        // updating color buffer only if zbuffer is ok
        int lZ = *((short int *)zbuf);

        if ((lZ > tz) && (tz > 0))
        {
            *((long*)p) = *((long*)tex + (long)tv * (tnext_scan >> 2) + (long)tu);
            *((short int *)zbuf) = (short int)tz;
            //cout << tz << " ";
        }

        //*((long*)p) = *((long*)(tex + (long)tv * tnext_scan + (long)tu));
        //cout << "texel dumped" << endl;

        tu += tdu;
        tv += tdv;
        tz += tdz;
        p  += 4;
        zbuf += 2;
        //cout << tz << " ";
    }

    //cout << endl;

    //cout << "end htexline" << endl;
}
//---------------------------------------------------------------------------

// Draws textured triangles
void ZTexTriangle(char *p, int next_scan, int p0x, int p0y, int p1x, int p1y, int p2x, int p2y,
                  char *tex, int tnext_scan, short int t0x, short int t0y, short int t1x, short int t1y, short int t2x, short int t2y,
                  char *zbuf, int znext_scan, float zb0, float zb1, float zb2,
                  int twidth, int theight)
{
    float   d, d0;
    float   x, x0;
    int     y;
    float   texUD, texUD0, texVD, texVD0;
    float   u, u0, v, v0;
    float   dz, dz0, z, z0;

    //
    //  sort points so P0.y <= P1.y <= P2.y
    //
    if (p0y > p1y)
    {
        SWAP(p0x,p1x);
        SWAP(p0y, p1y);
        SWAP(t0x, t1x);
        SWAP(t0y, t1y);
        SWAP(zb0, zb1);
    }
    if (p0y > p2y)
    {
        SWAP(p0x,p2x);
        SWAP(p0y, p2y);
        SWAP(t0x, t2x);
        SWAP(t0y, t2y);
        SWAP(zb0, zb2);
    }
    if (p1y > p2y)
    {
        SWAP(p1x,p2x);
        SWAP(p1y, p2y);
        SWAP(t1x, t2x);
        SWAP(t1y, t2y);
        SWAP(zb1, zb2);
    }

    //
    // check for fast exit on y
    //
    if (p0y > YBot || p2y < 0)
    {
        return;
    }

    //
    //  check for quick out?
    //
    /*
    if (p2y - p0y == 0)
    {
        return;
    }
    */

    //
    //  compute "long" side walk from P0 to P2
    //
    if ((p2y - p0y)!=0)
    {
        d = (float)(p2x - p0x) / (float)(p2y - p0y);
        texUD = (float)(t2x - t0x) / (float)(p2y - p0y);
        texVD = (float)(t2y - t0y) / (float)(p2y - p0y);
        dz = (float)(zb2 - zb0) / (float)(p2y - p0y);
    }
    else
    {
        d     = (p2x - p0x) * 100.;
        texUD = (float)(t2x - t0x) * 1000.0;
        texVD = (float)(t2y - t0y) * 1000.0;
        dz    = (float)(zb2 - zb0) * 1000.0;
    }


    x  = p0x;
    y  = p0y;
    u  = t0x;
    v  = t0y;
    z  = zb0;

    //p += p0y * next_scan;   // point p to correct scan.

    //
    //  do the top
    //
    if (p0y < p1y)
    {
        if ((p1y - p0y) != 0)
        {
            d0 = (float)(p1x - p0x) / (float)(p1y - p0y);
            texUD0 = (float)(t1x - t0x) / (float)(p1y - p0y);
            texVD0 = (float)(t1y - t0y) / (float)(p1y - p0y);
            dz0    = (float)(zb1 - zb0) / (float)(p1y - p0y);
        }
        else
        {
            d0 = (p1x - p0x) * 100.;
            texUD0 = (float)(t1x - t0x) * 1000.0;
            texVD0 = (float)(t1y - t0y) * 1000.0;
            dz0    = (float)(zb1 - zb0) * 1000.0;
        }

        x0 = p0x;
        u0 = t0x;
        v0 = t0y;
        z0 = zb0;

        //
        // check for left or right fill
        //
        if (d < d0)
        {
            while (y < p1y)
            {
                hZTexLine(p, next_scan, (int)x, (int)x0, y, tex, tnext_scan, (short int)u % twidth, (short int)u0 % twidth, (short int)v % theight, (short int)v0 % theight, zbuf, znext_scan, z, z0);
                y++;
                //p  += next_scan;
                x  += d;
                x0 += d0;
                u0 += texUD0;
                v0 += texVD0;
                u += texUD;
                v += texVD;
                z += dz;
                z0 += dz0;
            }
        }
        else
        {
            while (y < p1y)
            {
                hZTexLine(p, next_scan, (int)x, (int)x0, y, tex, tnext_scan, (short int)u % twidth, (short int)u0 % twidth, (short int)v % theight, (short int)v0 % theight, zbuf, znext_scan, z, z0);
                y++;
                //p  += next_scan;
                x  += d;
                x0 += d0;
                u0 += texUD0;
                v0 += texVD0;
                u += texUD;
                v += texVD;
                z += dz;
                z0 += dz0;
            }
        }
    }

    //
    // do the bottom.
    //

    /*
    if (p2y - p1y == 0)
    {
        return;
    }
    */

    if ((p2y - p1y) != 0)
    {
        d0 = (float)(p2x - p1x) / (float)(p2y - p1y);
        texUD0 = (float)(t2x - t1x) / (float)(p2y - p1y);
        texVD0 = (float)(t2y - t1y) / (float)(p2y - p1y);
        dz0    = (float)(zb2 - zb1) / (float)(p2y - p1y);
    }
    else
    {
        d0 = (p2x - p1x) * 100.;
        texUD0 = (float)(t2x - t1x) * 1000.;
        texVD0 = (float)(t2y - t1y) * 1000.;
        dz0    = (float)(zb2 - zb1) * 1000.;
    }

    x0 = p1x;
    u0 = t1x;
    v0 = t1y;
    z0 = zb1;

    //
    // check for left or right fill
    //
    if (d < d0)
    {
        while (y < p2y)
        {
            hZTexLine(p, next_scan, (int)x, (int)x0, y, tex, tnext_scan, (short int)u % twidth, (short int)u0 % twidth, (short int)v % theight, (short int)v0 % theight, zbuf, znext_scan, z, z0);
            y++;
            x  += d;
            x0 += d0;
            u0 += texUD0;
            v0 += texVD0;
            u += texUD;
            v += texVD;
            z += dz;
            z0 += dz0;
        }
    }
    else
    {
        while (y < p2y)
        {
            hZTexLine(p, next_scan, (int)x, (int)x0, y, tex, tnext_scan, (short int)u % twidth, (short int)u0 % twidth, (short int)v % theight, (short int)v0 % theight, zbuf, znext_scan, z, z0);
            //hZTexLine(p, next_scan, (int)x, (int)x0, y, tex, tnext_scan, (short int)u, (short int)u0, (short int)v, (short int)v0, zbuf, znext_scan, z, z0);
            y++;
            x  += d;
            x0 += d0;
            u0 += texUD0;
            v0 += texVD0;
            u += texUD;
            v += texVD;
            z += dz;
            z0 += dz0;
        }
    }
}
//---------------------------------------------------------------------------

// Descriere   : z buffer gouraud & texture horizontal line
void inline hZGTexLine(char *p,   long next_scan,  int xs, int xe, int y,
                       char *tex, long tnext_scan, short int tus, short int tue, short int tvs, short int tve,
                       char *zbuf, long znext_scan, float zs, float ze,
                       float rs, float gs, float bs, float re, float ge, float be,
                       int twidth, int theight)
{
    //cout << "htexline" << endl;

    // nu desenam puncte - astea se fac cu putPixel :-))
    if (xe == xs) return;

    // inversam daca s'au dat invers
    if (xe < xs)
    {
        SWAP(xe,xs);
        SWAP(tue,tus);
        SWAP(tvs, tve);
        SWAP(zs, ze);
        SWAP(re,rs);
        SWAP(ge,gs);
        SWAP(be,bs);
    }

    // teste de interioritate in fereastra
    //cout << YBot << ", " << YTop << endl;
    //cout << XRight << ", " << XLeft << endl;

    if ((y  < YTop) ||   (y  > YBot)) return;
    if ((xs > XRight) || (xe < XLeft)) return;

    int tuLength = tue - tus;
    int tvLength = tve - tvs;
    int xLength = xe - xs;
    float rLength = re - rs;
    float gLength = ge - gs;
    float bLength = be - bs;

    if (xs  < XLeft)
    {
        // update the starting texture coordinate also
        int deltaX = XLeft - xs;

        xs = XLeft;

        tus += (float)deltaX * tuLength / xLength;
        tvs += (float)deltaX * tvLength / xLength;

        rs += (float)deltaX * rLength / xLength;
        gs += (float)deltaX * gLength / xLength;
        bs += (float)deltaX * bLength / xLength;
    }

    if (xe  > XRight)
    {
        // update the end texture coordinate also
        xe = XRight;

        int deltaX = xe - XRight;

        tue = tue - (float)deltaX * tuLength / xLength;
        tve = tve - (float)deltaX * tvLength / xLength;

        re = re - (float)deltaX * rLength / xLength;
        ge = ge - (float)deltaX * gLength / xLength;
        be = be - (float)deltaX * bLength / xLength;
    }

    // recalculate after clipping
    xLength = xe - xs;

    p += y * next_scan + (xs * 4);
    zbuf = zbuf + y * znext_scan + (xs * 2);
    //tex += ty * tnext_scan + (txs * 4);

    float tdu = (float)(tue - tus) / xLength;
    float tdv = (float)(tve - tvs) / xLength;
    float  tu = tus, tv = tvs;
    float  tz = zs, tdz = (float)(ze - zs) / xLength;
    float rd = (float)(re - rs) / xLength;
    float gd = (float)(ge - gs) / xLength;
    float bd = (float)(be - bs) / xLength;

    //cout << "htex" << endl;
    //cout << zs << ", " << ze << endl;
    //cout << znext_scan << " ";

    //if (y > 239)
    //{
    //  cout << y << " :!";
    //}
    //else
    //{
    //  cout << y << " :";
    //}

    for(int i = 0; i < xLength; i++)
    {
        //cout << tu << ", " << tv << " - " << tnext_scan << " - " << tv * tnext_scan + tu << endl;

        // updating color buffer only if zbuffer is ok

        int lZ = *((short int *)zbuf);

        if ((lZ > tz) && (tz > 0))
        {
            // BUGFIX: there is crash (probably accessing out of bounds memory) with certain point configurations
            long lColor = *((long*)tex + (long)tv * (tnext_scan >> 2) + (long)tu);

            float lCr = rs * ((lColor & 0xFF0000) >> 16) / 255;
            float lCg = gs * ((lColor & 0x00FF00) >> 8) / 255;
            float lCb = bs * ((lColor & 0x0000FF)) / 255;

            long newColor = ((int)lCr << 16) | ((int)lCg << 8) | (int)lCb;

            *((long*)p) = newColor;
            *((short int *)zbuf) = (short int)tz;
            //cout << tz << " ";
        }

        //*((long*)p) = *((long*)(tex + (long)tv * tnext_scan + (long)tu));
        //cout << "texel dumped" << endl;

        tu += tdu;
        tu = std::clamp(tu, 0.f, (float)twidth);

        tv += tdv;
        tv = std::clamp(tv, 0.f, (float)theight);

        tz += tdz;
        p  += 4;
        zbuf += 2;
        rs += rd;
        gs += gd;
        bs += bd;
        //cout << tz << " ";
    }

    //cout << endl;

    //cout << "end htexline" << endl;
}
//---------------------------------------------------------------------------

// Draws gouraud & textured triangles
void ZGTexTriangle(char *p, int next_scan, int p0x, int p0y, int p1x, int p1y, int p2x, int p2y,
                   char *tex, int tnext_scan, short int t0x, short int t0y, short int t1x, short int t1y, short int t2x, short int t2y,
                   char *zbuf, int znext_scan, float zb0, float zb1, float zb2,
                   int twidth, int theight,
                   long color0, long color1, long color2)
{
    float   d, d0;
    float   x, x0;
    int     y;
    float   texUD, texUD0, texVD, texVD0;
    float   u, u0, v, v0;
    float   dz, dz0, z, z0;

    //
    //  sort points so P0.y <= P1.y <= P2.y
    //
    if (p0y > p1y)
    {
        SWAP(p0x,p1x);
        SWAP(p0y, p1y);
        SWAP(t0x, t1x);
        SWAP(t0y, t1y);
        SWAP(zb0, zb1);
        SWAP(color0, color1);
    }

    if (p0y > p2y)
    {
        SWAP(p0x,p2x);
        SWAP(p0y, p2y);
        SWAP(t0x, t2x);
        SWAP(t0y, t2y);
        SWAP(zb0, zb2);
        SWAP(color0, color2);
    }

    if (p1y > p2y)
    {
        SWAP(p1x,p2x);
        SWAP(p1y, p2y);
        SWAP(t1x, t2x);
        SWAP(t1y, t2y);
        SWAP(zb1, zb2);
        SWAP(color1, color2);
    }

    //
    // check for fast exit on y
    //
    if (p0y > YBot || p2y < 0)
    {
        return;
    }

    //
    //  check for quick out?
    //
    /*
    if (p2y - p0y == 0)
    {
        return;
    }
    */

    //
    // unfold the rgb colors
    //
    float r0 = (color0 & 0xFF0000) >> 16;
    float g0 = (color0 & 0x00FF00) >> 8;
    float b0 = (color0 & 0x0000FF);

    float r1 = (color1 & 0xFF0000) >> 16;
    float g1 = (color1 & 0x00FF00) >> 8;
    float b1 = (color1 & 0x0000FF);

    float r2 = (color2 & 0xFF0000) >> 16;
    float g2 = (color2 & 0x00FF00) >> 8;
    float b2 = (color2 & 0x0000FF);

    float rd, gd, bd;
    float rd0, gd0, bd0;

    //
    //  compute "long" side walk from P0 to P2
    //
    if ((p2y - p0y)!=0)
    {
        d = (float)(p2x - p0x) / (float)(p2y - p0y);
        texUD = (float)(t2x - t0x) / (float)(p2y - p0y);
        texVD = (float)(t2y - t0y) / (float)(p2y - p0y);
        dz = (float)(zb2 - zb0) / (float)(p2y - p0y);
        rd = (r2 - r0) / (float)(p2y - p0y);
        gd = (g2 - g0) / (float)(p2y - p0y);
        bd = (b2 - b0) / (float)(p2y - p0y);
    }
    else
    {
        d     = (p2x - p0x) * 100.;
        texUD = (float)(t2x - t0x) * 1000.0;
        texVD = (float)(t2y - t0y) * 1000.0;
        dz    = (float)(zb2 - zb0) * 1000.0;
        rd = (r2 - r0)   * 100.;
        gd = (g2 - g0)   * 100.;
        bd = (b2 - b0)   * 100.;
    }


    x  = p0x;
    y  = p0y;
    u  = t0x;
    v  = t0y;
    z  = zb0;
    float r = r0, g = g0, b = b0;

    //p += p0y * next_scan;   // point p to correct scan.

    //
    //  do the top
    //
    if (p0y < p1y)
    {
        if ((p1y - p0y) != 0)
        {
            d0 = (float)(p1x - p0x) / (float)(p1y - p0y);
            texUD0 = (float)(t1x - t0x) / (float)(p1y - p0y);
            texVD0 = (float)(t1y - t0y) / (float)(p1y - p0y);
            dz0    = (float)(zb1 - zb0) / (float)(p1y - p0y);
            rd0 = (r1 - r0) / (float)(p1y - p0y);
            gd0 = (g1 - g0) / (float)(p1y - p0y);
            bd0 = (b1 - b0) / (float)(p1y - p0y);
        }
        else
        {
            d0 = (p1x - p0x) * 100.;
            texUD0 = (float)(t1x - t0x) * 1000.0;
            texVD0 = (float)(t1y - t0y) * 1000.0;
            dz0    = (float)(zb1 - zb0) * 1000.0;
            rd0 = (r1 - r0) * 100.;
            gd0 = (g1 - g0) * 100.;
            bd0 = (b1 - b0) * 100.;
        }

        x0 = p0x;
        u0 = t0x;
        v0 = t0y;
        z0 = zb0;
        float rx = r0, gx = g0, bx = b0;

        //
        // check for left or right fill
        //
        if (d < d0)
        {
            while (y < p1y)
            {
                hZGTexLine(p, next_scan, (int)x, (int)x0, y, tex, tnext_scan, (short int)u % twidth, (short int)u0 % twidth, (short int)v % theight, (short int)v0 % theight, zbuf, znext_scan, z, z0, r, g, b, rx, gx, bx, twidth, theight);
                y++;
                //p  += next_scan;
                x  += d;
                x0 += d0;
                u0 += texUD0;
                v0 += texVD0;
                u += texUD;
                v += texVD;
                z += dz;
                z0 += dz0;
                r += rd;
                g += gd;
                b += bd;
                rx += rd0;
                gx += gd0;
                bx += bd0;
            }
        }
        else
        {
            while (y < p1y)
            {
                hZGTexLine(p, next_scan, (int)x, (int)x0, y, tex, tnext_scan, (short int)u % twidth, (short int)u0 % twidth, (short int)v % theight, (short int)v0 % theight, zbuf, znext_scan, z, z0, r, g, b, rx, gx, bx, twidth, theight);
                y++;
                //p  += next_scan;
                x  += d;
                x0 += d0;
                u0 += texUD0;
                v0 += texVD0;
                u += texUD;
                v += texVD;
                z += dz;
                z0 += dz0;
                r += rd;
                g += gd;
                b += bd;
                rx += rd0;
                gx += gd0;
                bx += bd0;
            }
        }
    }

    //
    // do the bottom.
    //

    /*
    if (p2y - p1y == 0)
    {
        return;
    }
    */

    if ((p2y - p1y) != 0)
    {
        d0 = (float)(p2x - p1x) / (float)(p2y - p1y);
        texUD0 = (float)(t2x - t1x) / (float)(p2y - p1y);
        texVD0 = (float)(t2y - t1y) / (float)(p2y - p1y);
        dz0    = (float)(zb2 - zb1) / (float)(p2y - p1y);
        rd0 = (r2 - r1) / (float)(p2y - p1y);
        gd0 = (g2 - g1) / (float)(p2y - p1y);
        bd0 = (b2 - b1) / (float)(p2y - p1y);
    }
    else
    {
        d0 = (p2x - p1x) * 100.;
        texUD0 = (float)(t2x - t1x) * 1000.;
        texVD0 = (float)(t2y - t1y) * 1000.;
        dz0    = (float)(zb2 - zb1) * 1000.;
        rd0 = (r2 - r1) * 100.;
        gd0 = (g2 - g1) * 100.;
        bd0 = (b2 - b1) * 100.;
    }

    x0 = p1x;
    u0 = t1x;
    v0 = t1y;
    z0 = zb1;
    float rx = r1, gx = g1, bx = b1;

    //
    // check for left or right fill
    //
    if (d < d0)
    {
        while (y < p2y)
        {
            hZGTexLine(p, next_scan, (int)x, (int)x0, y, tex, tnext_scan, (short int)u % twidth, (short int)u0 % twidth, (short int)v % theight, (short int)v0 % theight, zbuf, znext_scan, z, z0, r, g, b, rx, gx, bx, twidth, theight);
            y++;
            x  += d;
            x0 += d0;
            u0 += texUD0;
            v0 += texVD0;
            u += texUD;
            v += texVD;
            z += dz;
            z0 += dz0;
            r += rd;
            g += gd;
            b += bd;
            rx += rd0;
            gx += gd0;
            bx += bd0;
        }
    }
    else
    {
        while (y < p2y)
        {
            hZGTexLine(p, next_scan, (int)x, (int)x0, y, tex, tnext_scan, (short int)u % twidth, (short int)u0 % twidth, (short int)v % theight, (short int)v0 % theight, zbuf, znext_scan, z, z0, r, g, b, rx, gx, bx, twidth, theight);
            //hZTexLine(p, next_scan, (int)x, (int)x0, y, tex, tnext_scan, (short int)u, (short int)u0, (short int)v, (short int)v0, zbuf, znext_scan, z, z0);
            y++;
            x  += d;
            x0 += d0;
            u0 += texUD0;
            v0 += texVD0;
            u += texUD;
            v += texVD;
            z += dz;
            z0 += dz0;
            r += rd;
            g += gd;
            b += bd;
            rx += rd0;
            gx += gd0;
            bx += bd0;
        }
    }
}
//---------------------------------------------------------------------------

