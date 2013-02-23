#ifndef CWOBJECT_H_12314asdfas123123
#define CWOBJECT_H_12314asdfas123123

#include "CWMath.h"

#define CW_OBJECT4DV1_MAX_VERTICES           1024  // 64
#define CW_OBJECT4DV1_MAX_POLYS              1024 // 128

// states for objects
#define CW_OBJECT4DV1_STATE_ACTIVE           0x0001
#define CW_OBJECT4DV1_STATE_VISIBLE          0x0002 
#define CW_OBJECT4DV1_STATE_CULLED           0x0004



#define CW_POLY4DV1_ATTR_2SIDED                0x00000001
#define CW_POLY4DV1_ATTR_TRANSPARENT           0x00000002
#define CW_POLY4DV1_ATTR_8BITCOLOR             0x00000004
#define CW_POLY4DV1_ATTR_RGB16                 0x00000008
#define CW_POLY4DV1_ATTR_RGB24                 0x00000010

/* already defined in CWMath.h
#define CW_POLY4DV1_ATTR_SHADE_MODE_PURE       0x00000020
#define CW_POLY4DV1_ATTR_SHADE_MODE_CONSTANT   0x00000020 // (alias)
#define CW_POLY4DV1_ATTR_SHADE_MODE_FLAT       0x00000040
#define CW_POLY4DV1_ATTR_SHADE_MODE_GOURAUD    0x00000080
#define CW_POLY4DV1_ATTR_SHADE_MODE_PHONG      0x00000100
#define CW_POLY4DV1_ATTR_SHADE_MODE_FASTPHONG  0x00000100 // (alias)
#define CW_POLY4DV1_ATTR_SHADE_MODE_TEXTURE    0x00000200 
*/

#define CW_PLX_COLOR_MODE_RGB_FLAG             0x00008000   // this poly uses RGB color
#define CW_PLX_COLOR_MODE_INDEXED_FLAG         0x00000000   // this poly uses an indexed 8-bit color 

#define CW_POLY4DV1_ATTR_RGB32                 0x00001000


#define CW_PLX_SHADE_MODE_MASK   0x6000   // mask to extract shading mode

// shading mode of polygon
#define CW_PLX_SHADE_MODE_PURE_FLAG      0x0000  // this poly is a constant color
#define CW_PLX_SHADE_MODE_CONSTANT_FLAG  0x0000  // alias
#define CW_PLX_SHADE_MODE_FLAT_FLAG      0x2000  // this poly uses flat shading
#define CW_PLX_SHADE_MODE_GOURAUD_FLAG   0x4000  // this poly used gouraud shading
#define CW_PLX_SHADE_MODE_PHONG_FLAG     0x6000  // this poly uses phong shading
#define CW_PLX_SHADE_MODE_FASTPHONG_FLAG 0x6000  // this poly uses phong shading (alias)


// transformation control flags
#define CW_TRANSFORM_LOCAL_ONLY       0  // perform the transformation in place on the
                                      // local/world vertex list 
#define CW_TRANSFORM_TRANS_ONLY       1  // perfrom the transformation in place on the 
                                      // "transformed" vertex list
#define CW_TRANSFORM_LOCAL_TO_TRANS   2  // perform the transformation to the local
                                      // vertex list, but store the results in the
                                      // transformed vertex list


class CWObject{

public:
	CWObject();

	int InitFromFile( char *sFileName,     // filename of plg file
				  VECTOR4D * pVecScale,     // initial scaling factors
				  VECTOR4D * pVecPosition,       // initial position
				  VECTOR4D * pVecRotaion);

public:
	int GetState() const { return m_iState; }	

	int GetPolyNum() const{ return m_iPolyNum; }

	const ST_POLY4DATA * GetPolyPointer(int iIndex) const { 
		if( iIndex >= m_iPolyNum ){
			return NULL;
		}
		return &m_stPolyDataList[iIndex];
	};

	const POINT4D * GetLocalPointList() const {
		return m_PointListLocal;
	}

	const POINT4D * GetTransPointList() const {
		return m_PointListTrans;
	}

	void TransForm( MATRIX4X4 * pStMatrix ,int iCoordType);


private:
	float  InitRadius();
	char * Get_Line_PLG(char * pBuffer, int iBufferSize, FILE *fp);


private:
	char m_sFileName[512];
	int  m_iId;           // numeric id of this object
	char m_sName[64];     // ASCII name of object just for kicks
	int  m_iState;        // state of object
	int  m_iAttr;         // attributes of object
	float m_fAvgRadius;  // average radius of object used for collision detection
	float m_fMaxRadius;  // maximum radius of object

	POINT4D m_PointWorldPosition;  // position of object in world

	VECTOR4D m_VecDirection;       // rotation angles of object in local
	// cords or unit direction vector user defined???

	VECTOR4D m_VecUX,m_VecUY,m_VecUZ;  // local axes to track full orientation
	// this is updated automatically during
	// rotation calls

	int m_iVerticeNum;   // number of vertices of this object

	POINT4D m_PointListLocal[CW_OBJECT4DV1_MAX_VERTICES]; // array of local vertices
	POINT4D m_PointListTrans[CW_OBJECT4DV1_MAX_VERTICES]; // array of transformed vertices

	int m_iPolyNum;        // number of polygons in object mesh
	ST_POLY4DATA m_stPolyDataList[CW_OBJECT4DV1_MAX_POLYS];  // array of polygons
};


#endif