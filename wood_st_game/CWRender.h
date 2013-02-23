#ifndef CWRENDER_H_12314asdfas123123
#define CWRENDER_H_12314asdfas123123

#include "CWMath.h"
#include "CWDraw.h"
#include "CWObject.h"
#include "CWCamera.h"

// render list defines
#define CW_RENDERLIST4DV1_MAX_POLYS          32768// 16384


class CWRender{

public:
	CWRender();

	int Reset();
	
	int AddPoly4Rend( const ST_POLY4REND & rStPoly ); // <0 error, >=1 the polynum in render list now.

	int AddPoly4Data( const ST_POLY4DATA & rStPoly ); // <0 error, >=1 the polynum in render list now.

	int AddObject( const CWObject & rWObject , int iInsertLocal); // <0 error, >=1 the polynum in render list now.

	//comm compution...
	int Transform(MATRIX4X4 * pStMatrix,int iCoordType);


	//TransToWorld;
	int ModelToWorld(POINT4D * pStPointWordPos, int iCoordType);


	// apply world to camera transform
	int WorldToCamera( CWCamera * pWCamera );

	// apply camera to perspective transformation
	int CameraToPerspective( CWCamera * pWCamera );

	// apply screen transform
	int PerspectiveToScreen( CWCamera * pWCamera );

	int RendItOut( CWDraw & rWDraw );

private:

	int m_iState;
	int m_iAttr;  //attribute

	// the render list is an array of pointers each pointing to 
	// a self contained "renderable" polygon face POLYF4DV1
	ST_POLY4REND * m_pPolyArray[CW_RENDERLIST4DV1_MAX_POLYS];

	// additionally to cut down on allocatation, de-allocation
	// of polygons each frame, here's where the actual polygon
	// faces will be stored
	ST_POLY4REND m_stPolyRendData[CW_RENDERLIST4DV1_MAX_POLYS];

	int m_iPolyNum; // number of polys in render list

};

#endif