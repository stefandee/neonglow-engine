#ifndef OBJLIBH
#define OBJLIBH

#include "data3d.h"

class COLObject
{
  public:
    COLObject() {};
    ~COLObject() {};
  
  public:// ops 
    virtual CObject3D* Generate() = 0;    
    
  protected:
  
  private:
};

class COLCube : public COLObject
{
  public: // c-d
    COLCube();
    COLCube(CPoint3D _center, float _size, int _faceDetail);
    ~COLCube() {};
    
  public: // ops
    virtual CObject3D* Generate();  
    
  protected:
  
  private:
    int mFaceDetail;
    float mSize;
    CPoint3D mCenter;
};

class COLSphere : public COLObject
{
  public: // c-d
    COLSphere();
    COLSphere(CPoint3D _center, float _radius, int _faceDetail);
    ~COLSphere() {};
    
  public: // ops
    virtual CObject3D* Generate();  
    
  protected:
  
  private:
    int mFaceDetail;
    float mRadius;
    CPoint3D mCenter;
};

class COLCylinder : public COLObject
{
  public: // c - d
    COLCylinder();
    COLCylinder(CPoint3D _center, float _height, float _radius, int _details);
    ~COLCylinder() {};
    
  public: // ops
    virtual CObject3D* Generate();  
    
  private:
    CPoint3D mCenter;
    float    mHeight;  
    float    mRadius;
    int      mDetails;
};

class COLCone : public COLObject
{
  public: // c-d
    COLCone();
    COLCone(CPoint3D _center, float _height, float _radius, int _details);
    ~COLCone() {};
    
  public: // ops
    virtual CObject3D* Generate();  
    
  private:
    CPoint3D mCenter;
    float    mHeight;  
    float    mRadius;
    int      mDetails;
};

class COLTorus : public COLObject
{
  public: // c-d
    COLTorus();
    COLTorus(CPoint3D _center, float _outRadius, float _inRadius, int _secDetails, int skelDetails);
    ~COLTorus() {};
    
  public: // ops
    virtual CObject3D* Generate();  
    
  private:
    CPoint3D mCenter;
    float    mOuterRadius;  
    float    mInnerRadius;
    int      mSectionDetails, mSkeletonDetails;
};


//------------------------------------
//--- AICI AM COMPLETAT EU
//------------------------------------
class COLPQTorus : public COLObject
{
  public: // c-d
    COLPQTorus();
    COLPQTorus(CPoint3D _center, float _outRadius, float _inRadius, int _secDetails, int skelDetails, float mPParameter, float mQParameter);
    ~COLPQTorus() {};
    
  public: // ops
    virtual CObject3D* Generate();  
    
  private:
    CPoint3D mCenter;
    float    mOuterRadius;  
    float    mInnerRadius;
    int      mSectionDetails, mSkeletonDetails;
    float    mPParameter, mQParameter;
};

class COLTrunchiCone : public COLObject
{
  public: // c-d
    COLTrunchiCone();
    COLTrunchiCone(CPoint3D _center, float _height, float _smallradius, float _bigradius, int _details);
    ~COLTrunchiCone() {};
    
  public: // ops
    virtual CObject3D* Generate();  
    
  private:
    CPoint3D mCenter;
    float    mSmallRadius;  
    float    mBigRadius;
    float    mHeight , mDetails ;

};

class COLPiramid : public COLObject
{
  public: // c-d
    COLPiramid();
    COLPiramid(CPoint3D _center, float _height, float laturabazei, int _details);
    ~COLPiramid() {};
    
  public: // ops
    virtual CObject3D* Generate();  
    
  private:
    CPoint3D mCenter;
    float    mDetails;  
    float    mBaseSideSize;
    float    mHeight;

};

#endif
