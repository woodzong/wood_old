#ifndef CWCamera_H_12314asdfas123123
#define CWCamera_H_12314asdfas123123

#include "CWMath.h"



// defines for camera rotation sequences
#define CW_CAM_ROT_SEQ_XYZ  0
#define CW_CAM_ROT_SEQ_YXZ  1
#define CW_CAM_ROT_SEQ_XZY  2
#define CW_CAM_ROT_SEQ_YZX  3
#define CW_CAM_ROT_SEQ_ZYX  4
#define CW_CAM_ROT_SEQ_ZXY  5



#define CW_CAM_MODEL_EULER            0x0008
#define CW_CAM_MODEL_UVN              0x0010


// camera version 1
typedef struct CW_ST_CAMERA_TYP
{
	int iState;      // state of camera
	int iAttr;       // camera attributes

	POINT4D stPointPosition;    // world position of camera used by both camera models

	VECTOR4D stVecDirection;   // angles or look at direction of camera for simple 
	// euler camera models, elevation and heading for
	// uvn model

	VECTOR4D stVecU;     // extra vectors to track the camera orientation
	VECTOR4D stVecV;     // for more complex UVN camera model
	VECTOR4D stVecN;        

	VECTOR4D vecTarget; // look at target

	float fViewDistance;  // focal length 

	float fFieldOfView;          // field of view for both horizontal and vertical axes

	// 3d clipping planes
	// if view volume is NOT 90 degree then general 3d clipping
	// must be employed
	float fNearClipZ;     // near z=constant clipping plane
	float fFarClipZ;      // far z=constant clipping plane

	PLANE3D stPlaneRightClip;  // the right clipping plane
	PLANE3D stPlaneLeftClip;;  // the left clipping plane
	PLANE3D stPlaneTopClip;  // the top clipping plane
	PLANE3D stPlaneBottomClip;  // the bottom clipping plane                        

	float fViewPlaneWidth;     // width and height of view plane to project onto
	float fViewPlaneHeight;    // usually 2x2 for normalized projection or 
	// the exact same size as the viewport or screen window

	// remember screen and viewport are synonomous 
	float fViewPortWidth;     // size of screen/viewport
	float fViewPortHeight;
	float fViewPortCenterX;  // center of view port (final image destination)
	float fViewPortCenterY;

	// aspect ratio
	float fAspectRatio;

	// these matrices are not necessarily needed based on the method of
	// transformation, for example, a manual perspective or screen transform
	// and or a concatenated perspective/screen, however, having these 
	// matrices give us more flexibility         

	MATRIX4X4 stMatrixCam;   // storage for the world to camera transform matrix
	MATRIX4X4 stMatrixPer;   // storage for the camera to perspective transform matrix
	MATRIX4X4 stMatrixScr;   // storage for the perspective to screen transform matrix

} ST_CAMERA;


class CWCamera{
public:
	CWCamera();

	int Init(int _iAttr,POINT4D * pPointPosition,VECTOR4D * pVecDirection,POINT4D * pPointTarget,
			 float _fNearClipZ,float _fFarClipZ,float _fFieldOfView, float _fViewPortWidth,float _fViewPortHeight);

	const ST_CAMERA * GetCameraDataPointer() const{ 
			return & m_stCamera; 
	};

	
	void MakeMatrixEuler( int cam_rot_seq );

private:
	bool m_bIsInit;
	ST_CAMERA m_stCamera;
};


#endif