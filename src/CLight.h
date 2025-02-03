//------------------------------------------------------------------------------
// This file is part of NeonGlow Engine.                                      //
//                                                                            //
// NeonGlow Engine - software rendering generic 3D engine                     //
// Copyright (C) 2003 Stefan Dicu                                             //
//                                                                            //
// Licensed under MIT license.                                                //
//------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  Unit              : Light base class definition
//
//  Version           : 1.0
//
//  Description       :
//    * this is the base class for the lights object
//    * all other lights will inherit this class (area, point, spot, distant)
//    * the meaning of parameters is:
//      - color: obvious
//      - intensity falloff: enables the fading of the light intensity with distance;
//        the fading is linear, for now
//      - max range: the maximum range where the light reaches; at light origin, the
//        intensity is a percent (intensity, see below) while at max range the intensity
//        is 0%
//      - intensity: is specified as a percent; the color is first boosted by this
//        percent
//      - direction: except for the point light, all other lights use orientation
//
//  Hystory           :
//    [25.10.2003] - [Karg] - class created
//-----------------------------------------------------------------------------

#ifndef CLightH
#define CLightH
//---------------------------------------------------------------------------

#include "color.h"
#include "geom.h"
#include "math.h"

class CLight
{
public: // c-d
    CLight();
    virtual ~CLight() {}

public: // get-set
    virtual void SetColor(CRGB _rgb)
    {
        mColor = _rgb;
    }
    virtual CRGB GetColor()
    {
        return mColor;
    }

    virtual void SetHasIntensityFalloff(bool _v)
    {
        mHasIntensityFalloff = _v;
    }
    virtual bool GetHasIntensityFalloff()
    {
        return mHasIntensityFalloff;
    }

    virtual void  SetMaxRange(float _v);
    virtual float GetMaxRange()
    {
        return mMaxRange;
    }

    virtual void SetIntensity(int _v);
    virtual int  GetIntensity()
    {
        return mIntensity;
    }

    //virtual void SetLensFlareOptions(CLensFlareOptions _v);
    //virtual CLensFlareOptions GetLensFlareOptions();

    virtual void SetPosition(CPoint3D _v)
    {
        mPosition = _v;
    }
    virtual CPoint3D GetPosition()
    {
        return mPosition;
    }

    virtual void SetDirection(CVector3D _v)
    {
        mDir = _v;
    }
    virtual CVector3D GetDirection()
    {
        return mDir;
    }

public:// ops
    virtual void Draw();

protected:
    CRGB              mColor;
    bool              mHasIntensityFalloff;
    float             mMaxRange;
    int               mIntensity;
    //CLensFlareOptions mLensOptions;
    CPoint3D          mPosition;
    CVector3D         mDir;

private:
};

#endif
