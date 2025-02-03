#include <math.h>
#include "objlib.h"
#include "math3d.h"

COLCube::COLCube()
{
    mCenter = CPoint3D(0, 0, 0);
    mSize = 10.0;
    mFaceDetail = 1;
}
//-----------------------------------------------------------------

COLCube::COLCube(CPoint3D _center, float _size, int _faceDetail)
{
    mFaceDetail = _faceDetail;
    mSize       = _size;
    mCenter     = _center;
}
//-----------------------------------------------------------------

CObject3D* COLCube::Generate()
{
    CVertex lPoints[8];
    float lHS = mSize / 2;

    // init the points
    lPoints[0].SetOriginal(CPoint3D(mCenter.GetX() - lHS, mCenter.GetY() - lHS, mCenter.GetZ() - lHS));
    lPoints[1].SetOriginal(CPoint3D(mCenter.GetX() - lHS, mCenter.GetY() + lHS, mCenter.GetZ() - lHS));
    lPoints[2].SetOriginal(CPoint3D(mCenter.GetX() + lHS, mCenter.GetY() + lHS, mCenter.GetZ() - lHS));
    lPoints[3].SetOriginal(CPoint3D(mCenter.GetX() + lHS, mCenter.GetY() - lHS, mCenter.GetZ() - lHS));
    lPoints[4].SetOriginal(CPoint3D(mCenter.GetX() - lHS, mCenter.GetY() - lHS, mCenter.GetZ() + lHS));
    lPoints[5].SetOriginal(CPoint3D(mCenter.GetX() - lHS, mCenter.GetY() + lHS, mCenter.GetZ() + lHS));
    lPoints[6].SetOriginal(CPoint3D(mCenter.GetX() + lHS, mCenter.GetY() + lHS, mCenter.GetZ() + lHS));
    lPoints[7].SetOriginal(CPoint3D(mCenter.GetX() + lHS, mCenter.GetY() - lHS, mCenter.GetZ() + lHS));

    // alloc the object
    CObject3D* lObj = new CObject3D();

    // add the surfaces - 12 in number
    // bottom
    lObj->AddSurface(lPoints[0], lPoints[1], lPoints[2]);
    lObj->AddSurface(lPoints[0], lPoints[3], lPoints[2]);

    // top
    lObj->AddSurface(lPoints[4], lPoints[5], lPoints[6]);
    lObj->AddSurface(lPoints[4], lPoints[7], lPoints[6]);

    // left
    lObj->AddSurface(lPoints[0], lPoints[4], lPoints[7]);
    lObj->AddSurface(lPoints[0], lPoints[3], lPoints[7]);

    // right
    lObj->AddSurface(lPoints[1], lPoints[2], lPoints[6]);
    lObj->AddSurface(lPoints[1], lPoints[5], lPoints[6]);

    // front
    lObj->AddSurface(lPoints[3], lPoints[6], lPoints[7]);
    lObj->AddSurface(lPoints[3], lPoints[6], lPoints[2]);

    // back
    lObj->AddSurface(lPoints[0], lPoints[1], lPoints[5]);
    lObj->AddSurface(lPoints[0], lPoints[4], lPoints[5]);

    return lObj;
}
//-----------------------------------------------------------------

COLSphere::COLSphere()
{
    mCenter = CPoint3D(0, 0, 0);
    mRadius = 50;
    mFaceDetail = 1;
}
//-----------------------------------------------------------------

COLSphere::COLSphere(CPoint3D _center, float _radius, int _faceDetail)
{
    mCenter = _center;
    mRadius = _radius;
    mFaceDetail = _faceDetail;
}
//-----------------------------------------------------------------

CObject3D* COLSphere::Generate()
{
    int      i = 0;
    //CVertex lPrev[200], lCrt[200];
    CVertex* lPrev, *lCrt;
    CObject3D* lObj;

    int lPoints = (int)(360.0 / mFaceDetail) + 1;

    lPrev = new CVertex[lPoints];
    lCrt  = new CVertex[lPoints];

    for(float phi = 0.0; phi <= 360.0; phi += 360.0 / mFaceDetail)
    {
        lPrev[i].SetOriginal(
            CPoint3D(
                mCenter.GetX() + mRadius * sin(0) * cos(phi * M_PI / 180),
                mCenter.GetY() + mRadius * sin(0) * sin(phi * M_PI / 180),
                mCenter.GetZ() + mRadius * cos(0))
        );
        i++;
    }

    // to make the full circle
    lPrev[i] = lPrev[0];
    i++;

    lObj = new CObject3D();

    for(float theta = 0/*180.0 / (mFaceDetail * 2)*/; theta <= 90.0; theta += 180.0 / (mFaceDetail * 2))
    {
        i = 0;
        for(float phi = 0.0; phi < 360.0; phi += 360.0 / mFaceDetail)
        {
            lCrt[i].SetOriginal(
                CPoint3D(
                    mCenter.GetX() + mRadius * sin(theta * M_PI / 180) * cos(phi * M_PI / 180),
                    mCenter.GetY() + mRadius * sin(theta * M_PI / 180) * sin(phi * M_PI / 180),
                    mCenter.GetZ() + mRadius * cos(theta * M_PI / 180))
            );
            i++;
        }

        // to make the full circle
        lCrt[i] = lCrt[0];
        i++;

        for(int j = 0; j < i - 1; j++)
        {
            // add first surfaces
            lObj->AddSurface(lPrev[j], lPrev[j + 1], lCrt[j]);
            lObj->AddSurface(lCrt[j], lCrt[j + 1], lPrev[j + 1]);

            // compute mirror by Z surfaces
            CVertex lMirror[4];

            lMirror[0].SetOriginal(CPoint3D(lPrev[j].GetOriginal().GetX(), lPrev[j].GetOriginal().GetY(), -lPrev[j].GetOriginal().GetZ() + 2 * mCenter.GetZ()));
            lMirror[1].SetOriginal(CPoint3D(lPrev[j + 1].GetOriginal().GetX(), lPrev[j + 1].GetOriginal().GetY(), -lPrev[j + 1].GetOriginal().GetZ() + 2 * mCenter.GetZ()));
            lMirror[2].SetOriginal(CPoint3D(lCrt[j].GetOriginal().GetX(), lCrt[j].GetOriginal().GetY(), -lCrt[j].GetOriginal().GetZ() + 2 * mCenter.GetZ()));
            lMirror[3].SetOriginal(CPoint3D(lCrt[j + 1].GetOriginal().GetX(), lCrt[j + 1].GetOriginal().GetY(), -lCrt[j + 1].GetOriginal().GetZ() + 2 * mCenter.GetZ()));

            // add mirror by Z
            lObj->AddSurface(lMirror[0], lMirror[1], lMirror[2]);
            lObj->AddSurface(lMirror[2], lMirror[3], lMirror[1]);
        }

        // optimize later
        for(int j = 0; j < i; j++)
        {
            lPrev[j] = lCrt[j];
        }
    }

    delete [] lPrev;
    delete [] lCrt;

    return lObj;
}
//-----------------------------------------------------------------

COLCylinder::COLCylinder()
{
    mCenter = CPoint3D(0, 0, 0);
    mHeight = 10.0;
    mRadius = 10.0;
    mDetails = 3;
}
//-----------------------------------------------------------------

COLCylinder::COLCylinder(CPoint3D _center, float _height, float _radius, int _details)
{
    mCenter = _center;
    mHeight = _height;
    mRadius = _radius;
    mDetails = _details;
}
//-----------------------------------------------------------------

CObject3D* COLCylinder::Generate()
{
    CObject3D* lObj = new CObject3D();
    CVertex lPrevV1, lV1, lV2, lPrevV2, lF1, lF2, lTopCenter, lCenter;
    float lStep = 360.0 / mDetails;

    lCenter.SetOriginal(mCenter);
    lTopCenter.SetOriginal(CPoint3D(mCenter.GetX(), mCenter.GetY(), mCenter.GetZ() + mHeight));
    lPrevV1.SetOriginal(CPoint3D(mCenter.GetX() + mRadius, mCenter.GetY(), mCenter.GetZ()));
    lPrevV2.SetOriginal(CPoint3D(mCenter.GetX() + mRadius, mCenter.GetY(), mCenter.GetZ() + mHeight));

    lF1 = lPrevV1;
    lF2 = lPrevV2;

    for(float lAngle = lStep; lAngle < 360.0; lAngle += lStep)
    {
        lV1.SetOriginal(
            CPoint3D(
                mCenter.GetX() + mRadius * cos(lAngle * M_PI / 180),
                mCenter.GetY() + mRadius * sin(lAngle * M_PI / 180),
                mCenter.GetZ()
            )
        );

        lV2.SetOriginal(
            CPoint3D(
                mCenter.GetX() + mRadius * cos(lAngle * M_PI / 180),
                mCenter.GetY() + mRadius * sin(lAngle * M_PI / 180),
                mCenter.GetZ() + mHeight
            )
        );

        // add sides
        lObj->AddSurface(lV1, lPrevV1, lV2);
        lObj->AddSurface(lV2, lPrevV2, lPrevV1);

        // add top and bottom - it actually adds a polygon that's not needed (lF1, lF1, lV1)
        lObj->AddSurface(lCenter, lPrevV1, lV1);
        lObj->AddSurface(lTopCenter, lPrevV2, lV2);

        // prepare the next step
        lPrevV1 = lV1;
        lPrevV2 = lV2;
    }

    // link first and last
    lObj->AddSurface(lV1, lF1, lV2);
    lObj->AddSurface(lV2, lF2, lF1);
    lObj->AddSurface(lCenter, lF1, lV1);
    lObj->AddSurface(lTopCenter, lF2, lV2);

    return lObj;
}
//-----------------------------------------------------------------

COLCone::COLCone()
{
    mCenter = CPoint3D(0, 0, 0);
    mHeight = 10.0;
    mRadius = 10.0;
    mDetails = 3;
}
//-----------------------------------------------------------------

COLCone::COLCone(CPoint3D _center, float _height, float _radius, int _details)
{
    mCenter = _center;
    mHeight = _height;
    mRadius = _radius;
    mDetails = _details;
}
//-----------------------------------------------------------------

CObject3D* COLCone::Generate()
{
    CObject3D* lObj = new CObject3D();
    float   lStep = 360.0 / mDetails;
    CVertex lTop, lCenter, lPrev, lV, lF;

    lCenter.SetOriginal(mCenter);
    lTop.SetOriginal(CPoint3D(mCenter.GetX(), mCenter.GetY(), mCenter.GetZ() + mHeight));
    lF.SetOriginal(CPoint3D(mCenter.GetX() + mRadius, mCenter.GetY(), mCenter.GetZ()));
    lPrev = lF;

    for(float lAngle = lStep; lAngle < 360.0; lAngle += lStep)
    {
        lV.SetOriginal(
            CPoint3D(
                mCenter.GetX() + mRadius * cos(lAngle * M_PI / 180),
                mCenter.GetY() + mRadius * sin(lAngle * M_PI / 180),
                mCenter.GetZ()
            )
        );

        // add side
        lObj->AddSurface(lTop, lPrev, lV);

        // add bottom
        lObj->AddSurface(lCenter, lPrev, lV);

        lPrev = lV;
    }

    // complete the cone
    // add side
    lObj->AddSurface(lTop, lF, lV);

    // add bottom
    lObj->AddSurface(lCenter, lF, lV);

    return lObj;
}
//-----------------------------------------------------------------

COLTorus::COLTorus()
{
    mCenter = CPoint3D(0, 0, 0);
    mOuterRadius = 10.0;
    mInnerRadius = 8.0;
    mSectionDetails = 10;
    mSkeletonDetails = 10;
}
//-----------------------------------------------------------------

COLTorus::COLTorus(CPoint3D _center, float _outRadius, float _inRadius, int _secDetails, int _skelDetails)
{
    mCenter = _center;
    mOuterRadius = _outRadius;
    mInnerRadius = _inRadius;
    mSectionDetails = _secDetails;
    mSkeletonDetails = _skelDetails;
}
//-----------------------------------------------------------------

CObject3D* COLTorus::Generate()
{
    CObject3D* lObj = new CObject3D();
    CVertex*   lSection;
    float      lSectionAngle =  360.0 / mSectionDetails;
    float      lSkeletonAngle = 360.0 / mSkeletonDetails;
    float      lCenterRadius = mInnerRadius + (mOuterRadius - mInnerRadius) / 2.0;
    float      lSectionRadius = (mOuterRadius - mInnerRadius) / 2.0;

    lSection = new CVertex[mSectionDetails + 1];

    // compute first section
    for(int i = 0; i < mSectionDetails; i++)
    {
        lSection[i].SetOriginal
        (
            CPoint3D
            (
                lCenterRadius + mCenter.GetX() + lSectionRadius * cos(i * lSectionAngle * M_PI / 180.0),
                mCenter.GetY() + lSectionRadius * sin(i * lSectionAngle * M_PI / 180.0),
                mCenter.GetZ()
            )
        );
    }

    lSection[mSectionDetails] = lSection[0];

    CMatrix4 lRot;

    for(int j = 0; j < mSkeletonDetails; j++)
    {

        lRot.YRotation(lSkeletonAngle * M_PI / 180.0);

        for(int i = 0; i < mSectionDetails; i++)
        {
            // transform
            CVertex lNext1, lNext2;

            lNext1.SetOriginal(lRot.Transform(lSection[i].GetOriginal()));
            lNext2.SetOriginal(lRot.Transform(lSection[i + 1].GetOriginal()));

            // add surfaces to object
            lObj->AddSurface(lSection[i], lSection[i + 1], lNext1);

            lObj->AddSurface(lNext1, lNext2, lSection[i + 1]);

            // rotate the reference point
            lSection[i].SetOriginal(lRot.Transform(lSection[i].GetOriginal()));
        }

        lSection[mSectionDetails].SetOriginal(lRot.Transform(lSection[mSectionDetails].GetOriginal()));
    }

    return lObj;
}
//-----------------------------------------------------------------

