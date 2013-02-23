#include "CWCamera.h"


CWCamera::CWCamera(){
	m_bIsInit = false;
}


int CWCamera::Init(int _iAttr,
				   POINT4D * pPointPosition,
				   VECTOR4D * pVecDirection,
				   POINT4D * pPointTarget,
				   float _fNearClipZ,
				   float _fFarClipZ,
				   float _fFieldOfView,
				   float _fViewPortWidth,
				   float _fViewPortHeight)
{
	if(m_bIsInit){
		return 1;
	}

	m_stCamera.iAttr  = _iAttr;
	VECTOR4D_COPY( &m_stCamera.stPointPosition, pPointPosition); // positions
	VECTOR4D_COPY( &m_stCamera.stVecDirection,  pVecDirection); // direction vector or angles for

	// euler camera
	// for UVN camera
	VECTOR4D_INITXYZ(&m_stCamera.stVecU, 1,0,0);  // set to +x
	VECTOR4D_INITXYZ(&m_stCamera.stVecV, 0,1,0);  // set to +y
	VECTOR4D_INITXYZ(&m_stCamera.stVecN, 0,0,1);  // set to +z     

	if (pPointTarget!=NULL){
		VECTOR4D_COPY( &m_stCamera.vecTarget, pPointTarget ); // UVN target
	}
	else{
		VECTOR4D_ZERO( &m_stCamera.vecTarget );
	}

	m_stCamera.fNearClipZ = _fNearClipZ;
	m_stCamera.fFarClipZ  = _fFarClipZ;

	m_stCamera.fViewPortWidth  = _fViewPortWidth;
	m_stCamera.fViewPortHeight = _fViewPortHeight;

	m_stCamera.fViewPortCenterX = (_fViewPortWidth-1)/2;
	m_stCamera.fViewPortCenterY = (_fViewPortHeight-1)/2;

	m_stCamera.fAspectRatio = (float)_fViewPortWidth/(float)_fViewPortHeight;


	MAT_IDENTITY_4X4(&m_stCamera.stMatrixCam);
	MAT_IDENTITY_4X4(&m_stCamera.stMatrixPer);
	MAT_IDENTITY_4X4(&m_stCamera.stMatrixScr);

	m_stCamera.fFieldOfView = _fFieldOfView;

	// set the viewplane dimensions up, they will be 2 x (2/ar)
	m_stCamera.fViewPlaneWidth  = 2.0;
	m_stCamera.fViewPlaneHeight = (float)2.0/m_stCamera.fAspectRatio;

	// now we know fov and we know the viewplane dimensions plug into formula and
	// solve for view distance parameters
	float fTanFovDiv2 = (float)tan(DEG_TO_RAD(_fFieldOfView/2));
	m_stCamera.fViewDistance = (float)(0.5)*(m_stCamera.fViewPlaneWidth)*fTanFovDiv2;

	// test for 90 fov first since it's easy :)
	if (_fFieldOfView == 90.0)
	{
		// set up the clipping planes -- easy for 90 degrees!
		POINT3D stPointTmp; // point on the plane
		VECTOR3D_INITXYZ(&stPointTmp,0,0,0);

		VECTOR3D stVecTmpNormal; // normal to plane

		// right clipping plane 
		VECTOR3D_INITXYZ(&stVecTmpNormal,1,0,-1); // x=z plane
		CWMath::Plane3D_Init(&m_stCamera.stPlaneRightClip, &stPointTmp,  &stVecTmpNormal, 1);

		// left clipping plane
		VECTOR3D_INITXYZ(&stVecTmpNormal,-1,0,-1); // -x=z plane
		CWMath::Plane3D_Init(&m_stCamera.stPlaneLeftClip, &stPointTmp,  &stVecTmpNormal, 1);

		// top clipping plane
		VECTOR3D_INITXYZ(&stVecTmpNormal,0,1,-1); // y=z plane
		CWMath::Plane3D_Init(&m_stCamera.stPlaneTopClip, &stPointTmp,  &stVecTmpNormal, 1);

		// bottom clipping plane
		VECTOR3D_INITXYZ(&stVecTmpNormal,0,-1,-1); // -y=z plane
		CWMath::Plane3D_Init(&m_stCamera.stPlaneBottomClip, &stPointTmp,  &stVecTmpNormal, 1);
	} // end if d=1
	else 
	{
		// now compute clipping planes yuck!
		POINT3D stPointTmp; // point on the plane
		VECTOR3D_INITXYZ(&stPointTmp,0,0,0);

		VECTOR3D stVecTmpNormal; // normal to plane

		// since we don't have a 90 fov, computing the normals
		// are a bit tricky, there are a number of geometric constructions
		// that solve the problem, but I'm going to solve for the
		// vectors that represent the 2D projections of the frustrum planes
		// on the x-z and y-z planes and then find perpendiculars to them

		// right clipping plane, check the math on graph paper 
		VECTOR3D_INITXYZ(&stVecTmpNormal,(float)m_stCamera.fViewDistance,0,(float)-m_stCamera.fViewPlaneWidth/2.0); 
		CWMath::Plane3D_Init(&m_stCamera.stPlaneRightClip, &stPointTmp,  &stVecTmpNormal, 1);

		// left clipping plane, we can simply reflect the right normal about
		// the z axis since the planes are symetric about the z axis
		// thus invert x only
		VECTOR3D_INITXYZ(&stVecTmpNormal,(float)-m_stCamera.fViewDistance,0,(float)-m_stCamera.fViewPlaneWidth/2.0); 
		CWMath::Plane3D_Init(&m_stCamera.stPlaneLeftClip, &stPointTmp,  &stVecTmpNormal, 1);

		// top clipping plane, same construction
		VECTOR3D_INITXYZ(&stVecTmpNormal,0,(float)m_stCamera.fViewDistance,(float)-m_stCamera.fViewPlaneWidth/2.0); 
		CWMath::Plane3D_Init(&m_stCamera.stPlaneTopClip, &stPointTmp,  &stVecTmpNormal, 1);

		// bottom clipping plane, same inversion
		VECTOR3D_INITXYZ(&stVecTmpNormal,0,(float)-m_stCamera.fViewDistance,(float)-m_stCamera.fViewPlaneWidth/2.0); 
		CWMath::Plane3D_Init(&m_stCamera.stPlaneBottomClip, &stPointTmp,  &stVecTmpNormal, 1);
	} // end else

	m_bIsInit = true;
	return 0;
}


void CWCamera::MakeMatrixEuler( int cam_rot_seq )
{
	// this creates a camera matrix based on Euler angles 
	// and stores it in the sent camera object
	// if you recall from chapter 6 to create the camera matrix
	// we need to create a transformation matrix that looks like:

	// Mcam = mt(-1) * my(-1) * mx(-1) * mz(-1)
	// that is the inverse of the camera translation matrix mutilplied
	// by the inverses of yxz, in that order, however, the order of
	// the rotation matrices is really up to you, so we aren't going
	// to force any order, thus its programmable based on the value
	// of cam_rot_seq which can be any value CAM_ROT_SEQ_XYZ where 
	// XYZ can be in any order, YXZ, ZXY, etc.

	MATRIX4X4 mt_inv,  // inverse camera translation matrix
		mx_inv,  // inverse camera x axis rotation matrix
		my_inv,  // inverse camera y axis rotation matrix
		mz_inv,  // inverse camera z axis rotation matrix
		mrot,    // concatenated inverse rotation matrices
		mtmp;    // temporary working matrix


	// step 1: create the inverse translation matrix for the camera
	// position
	CWMath::Matrix4X4Init(&mt_inv, 
		1,    0,     0,     0,
		0,    1,     0,     0,
		0,    0,     1,     0,
		-m_stCamera.stPointPosition.x, -m_stCamera.stPointPosition.y, -m_stCamera.stPointPosition.z, 1);

	// step 2: create the inverse rotation sequence for the camera
	// rember either the transpose of the normal rotation matrix or
	// plugging negative values into each of the rotations will result
	// in an inverse matrix

	// first compute all 3 rotation matrices

	// extract out euler angles
	float theta_x = m_stCamera.stVecDirection.x;
	float theta_y = m_stCamera.stVecDirection.y;
	float theta_z = m_stCamera.stVecDirection.z;

	// compute the sine and cosine of the angle x
	float cos_theta = CW_MATH->FastCos(theta_x);  // no change since cos(-x) = cos(x)
	float sin_theta = -CW_MATH->FastSin(theta_x); // sin(-x) = -sin(x)

	// set the matrix up 
	CWMath::Matrix4X4Init(&mx_inv, 1,    0,         0,         0,
		0,    cos_theta, sin_theta, 0,
		0,   -sin_theta, cos_theta, 0,
		0,    0,         0,         1);

	// compute the sine and cosine of the angle y
	cos_theta = CW_MATH->FastCos(theta_y);  // no change since cos(-x) = cos(x)
	sin_theta = -CW_MATH->FastSin(theta_y); // sin(-x) = -sin(x)

	// set the matrix up 
	CWMath::Matrix4X4Init(&my_inv,
		cos_theta, 0, -sin_theta, 0,  
		0,         1,  0,         0,
		sin_theta, 0,  cos_theta,  0,
		0,         0,  0,          1);

	// compute the sine and cosine of the angle z
	cos_theta = CW_MATH->FastCos(theta_z);  // no change since cos(-x) = cos(x)
	sin_theta = -CW_MATH->FastSin(theta_z); // sin(-x) = -sin(x)

	// set the matrix up 
	CWMath::Matrix4X4Init(&mz_inv, 
		cos_theta, sin_theta, 0, 0,  
		-sin_theta, cos_theta, 0, 0,
		0,         0,         1, 0,
		0,         0,         0, 1);

	// now compute inverse camera rotation sequence
	switch(cam_rot_seq)
	{
	case CW_CAM_ROT_SEQ_XYZ:
		{
			CWMath::Matrix4X4Mul(&mx_inv, &my_inv, &mtmp);
			CWMath::Matrix4X4Mul(&mtmp, &mz_inv, &mrot);
		} break;

	case CW_CAM_ROT_SEQ_YXZ:
		{
			CWMath::Matrix4X4Mul(&my_inv, &mx_inv, &mtmp);
			CWMath::Matrix4X4Mul(&mtmp, &mz_inv, &mrot);
		} break;

	case CW_CAM_ROT_SEQ_XZY:
		{
			CWMath::Matrix4X4Mul(&mx_inv, &mz_inv, &mtmp);
			CWMath::Matrix4X4Mul(&mtmp, &my_inv, &mrot);
		} break;

	case CW_CAM_ROT_SEQ_YZX:
		{
			CWMath::Matrix4X4Mul(&my_inv, &mz_inv, &mtmp);
			CWMath::Matrix4X4Mul(&mtmp, &mx_inv, &mrot);
		} break;

	case CW_CAM_ROT_SEQ_ZYX:
		{
			CWMath::Matrix4X4Mul(&mz_inv, &my_inv, &mtmp);
			CWMath::Matrix4X4Mul(&mtmp, &mx_inv, &mrot);
		} break;

	case CW_CAM_ROT_SEQ_ZXY:
		{
			CWMath::Matrix4X4Mul(&mz_inv, &mx_inv, &mtmp);
			CWMath::Matrix4X4Mul(&mtmp, &my_inv, &mrot);

		} break;

	default: break;
	} // end switch

	// now mrot holds the concatenated product of inverse rotation matrices
	// multiply the inverse translation matrix against it and store in the 
	// camera objects' camera transform matrix we are done!
	CWMath::Matrix4X4Mul(&mt_inv, &mrot, &m_stCamera.stMatrixCam);
} // end Build_CAM4DV1_Matrix_Euler
