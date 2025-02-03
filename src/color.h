//------------------------------------------------------------------------------
// This file is part of NeonGlow Engine.                                      //
//                                                                            //
// NeonGlow Engine - software rendering generic 3D engine                     //
// Copyright (C) 2003 Stefan Dicu                                             //
//                                                                            //
// Licensed under MIT license.                                                //
//------------------------------------------------------------------------------

#ifndef colorH
#define colorH
//---------------------------------------------------------------------------

class CRGB
{
public:
    CRGB()
    {
        mColor = 0;
    }
    CRGB(int _color)
    {
        mColor = _color;
    }
    CRGB(int _r, int _g, int _b);
    ~CRGB() {}

public: //ops
    unsigned char GetR()
    {
        return (mColor & 0xFF0000) >> 16;
    }
    void          SetR(unsigned char _v)
    {
        mColor = (mColor & 0x0000FFFF) | ((int)_v << 16);
    }

    unsigned char GetG()
    {
        return (mColor & 0x00FF00) >> 8;
    }
    void          SetG(unsigned char _v)
    {
        mColor = (mColor & 0x00FF00FF) | ((int)_v << 8);
    }

    unsigned char GetB()
    {
        return (mColor & 0x000000FF);
    }
    void          SetB(unsigned char _v)
    {
        mColor = (mColor & 0x000000FF) | (int)_v;
    }

    int  GetColor()
    {
        return mColor;
    }
    void SetColor(int _v)
    {
        mColor = _v;
    }

protected:

private:
    int mColor;
};

#endif
