#include <ctype.h>
#include "CWoodLog.h"
#include "CWObject.h"






char *CWObject::Get_Line_PLG(char * pBuffer, int iBufferSize, FILE *fp)
{
	// this little helper function simply read past comments 
	// and blank lines in a PLG file and always returns full 
	// lines with something on them on NULL if the file is empty

	int index = 0;  // general index
	int length = 0; // general length

	// enter into parsing loop
	while(1)
	{
		// read the next line
		if (!fgets(pBuffer, iBufferSize, fp))
			return(NULL);

		// kill the whitespace
		for (length = strlen(pBuffer), index = 0; isspace(pBuffer[index]); index++);

		// test if this was a blank line or a comment
		if (index >= length || pBuffer[index]=='#') 
			continue;

		// at this point we have a good line
		return(&pBuffer[index]);
	} // end while

} // end Get_Line_PLG


float CWObject::InitRadius(){
	// this function computes the average and maximum radius for 
	// sent object and opdates the object data

	// reset incase there's any residue
	m_fAvgRadius = 0;
	m_fMaxRadius = 0;

	// loop thru and compute radius
	for (int iIndexVertex = 0; iIndexVertex < m_iVerticeNum; iIndexVertex++)
	{
		// update the average and maximum radius
		float fDistanceToVertex = 
			sqrt( m_PointListLocal[iIndexVertex].x * m_PointListLocal[iIndexVertex].x +
			m_PointListLocal[iIndexVertex].y * m_PointListLocal[iIndexVertex].y +
			m_PointListLocal[iIndexVertex].z * m_PointListLocal[iIndexVertex].z);

		// accumulate total radius
		m_fAvgRadius += fDistanceToVertex;

		// update maximum radius   
		if ( fDistanceToVertex > m_fMaxRadius ){
			m_fMaxRadius = fDistanceToVertex;
		}

	} // end for vertex

	// finallize average radius computation
	m_fAvgRadius /= m_iVerticeNum;

	// return max radius
	return m_fMaxRadius ;

} // end Compute_OBJECT4DV1_Radius


CWObject::CWObject(){
	memset(m_sFileName,0,sizeof(m_sFileName));
}

int CWObject::InitFromFile( char *sFileName,     // filename of plg file
						   VECTOR4D * pVecScale,     // initial scaling factors
						   VECTOR4D * pVecPosition,       // initial position
						   VECTOR4D * pVecRotaion)       // initial rotations
{

	CWOOD_LOG_DEBUG("InitFromFile Start: %s ", sFileName);

	// this function loads a plg object in off disk, additionally
	// it allows the caller to scale, position, and rotate the object
	// to save extra calls later for non-dynamic objects

	_snprintf_s(m_sFileName,sizeof(m_sFileName),sizeof(m_sFileName),"%s",sFileName);

	FILE *fp = NULL;          // file pointer
	char sTmpBuffer[256]  = {0};  // working buffer
	char *pTokenString = NULL;  // pointer to actual token text, ready for parsing

	// file format review, note types at end of each description
	// # this is a comment

	// # object descriptor
	// object_name_string num_verts_int num_polys_int

	// # vertex list
	// x0_float y0_float z0_float
	// x1_float y1_float z1_float
	// x2_float y2_float z2_float
	// .
	// .
	// xn_float yn_float zn_float
	//
	// # polygon list
	// surface_description_ushort num_verts_int v0_index_int v1_index_int ..  vn_index_int
	// .
	// .
	// surface_description_ushort num_verts_int v0_index_int v1_index_int ..  vn_index_int

	// lets keep it simple and assume one element per line
	// hence we have to find the object descriptor, read it in, then the
	// vertex list and read it in, and finally the polygon list -- simple :)

	// set state of object to active and visible
	m_iState = CW_OBJECT4DV1_STATE_ACTIVE | CW_OBJECT4DV1_STATE_VISIBLE;

	// set position of object
	m_PointWorldPosition.x = pVecPosition->x;
	m_PointWorldPosition.y = pVecPosition->y;
	m_PointWorldPosition.z = pVecPosition->z;
	m_PointWorldPosition.w = pVecPosition->w;

	CWOOD_LOG_DEBUG("fopen_s Start: %s ", sFileName);
	// Step 2: open the file for reading
	long lRetCode = 0;
	lRetCode = fopen_s(&fp,m_sFileName, "r");
	if ( lRetCode!=0 ){
		CWOOD_LOG_ERR_L(lRetCode,"Couldn't open PLG file %s.", m_sFileName);
		return lRetCode;
	} // end if

	CWOOD_LOG_DEBUG("fopen_s Start: %s ", sFileName);

	// Step 3: get the first token string which should be the object descriptor
	if (!(pTokenString = Get_Line_PLG(sTmpBuffer, 255, fp))){
		CWOOD_LOG_ERR_L(-2,"PLG file error with file %s (object descriptor invalid).",m_sFileName);
		return(0);
	} // end if

	CWOOD_LOG_DEBUG("Object Descriptor: %s ", pTokenString);

	CWOOD_LOG_DEBUG("Object Descriptor: %s SizeTest:%d", pTokenString,sizeof(m_sName));

	// parse out the info object
	sscanf_s(pTokenString, "%s %d %d", m_sName,sizeof(m_sName), &m_iVerticeNum, &m_iPolyNum);

	CWOOD_LOG_DEBUG("Object Descriptor2: %s", pTokenString);

	// Step 4: load the vertex list
	for (int iVertexIndex = 0; iVertexIndex < m_iVerticeNum; iVertexIndex++)
	{
		// get the next vertex
		if (!(pTokenString = Get_Line_PLG(sTmpBuffer, 255, fp))){
			CWOOD_LOG_ERR_L(-4,"PLG file error with file %s (vertex list invalid).",m_sFileName);
			return(0);
		} // end if

		// parse out vertex
		sscanf_s(pTokenString, "%f %f %f", &m_PointListLocal[iVertexIndex].x, &m_PointListLocal[iVertexIndex].y, &m_PointListLocal[iVertexIndex].z);
		m_PointListLocal[iVertexIndex].w = 1;    

		// scale vertices
		m_PointListLocal[iVertexIndex].x*=pVecScale->x;
		m_PointListLocal[iVertexIndex].y*=pVecScale->y;
		m_PointListLocal[iVertexIndex].z*=pVecScale->z;

		CWOOD_LOG_DEBUG("Vertex %d = %f, %f, %f, %f", iVertexIndex,
			m_PointListLocal[iVertexIndex].x, 
			m_PointListLocal[iVertexIndex].y, 
			m_PointListLocal[iVertexIndex].z,
			m_PointListLocal[iVertexIndex].w);

	} // end for vertex

	// compute average and max radius
	InitRadius();

	CWOOD_LOG_DEBUG("Object average radius = %f, max radius = %f",m_fAvgRadius,m_fMaxRadius);

	int iPolySurfaceDesc = 0; // PLG/PLX surface descriptor
	int iPolyNumVertexs  = 0; // number of vertices for current poly (always 3)
	int iPolyColor       = 0;
	char sTmpDescBuffer[12]  = {0};        // temp string to hold surface descriptor in and
	char sTmpColorBuffer[12] = {0};        // temp string to hold surface color in and
	// test if it need to be converted from hex

	// Step 5: load the polygon list
	for (int iPolyIndex=0; iPolyIndex < m_iPolyNum; iPolyIndex++ )
	{
		// get the next vertex
		if (!(pTokenString = Get_Line_PLG(sTmpBuffer, 255, fp))){
			CWOOD_LOG_ERR_L(-4,"PLG file error with file %s (vertex list invalid).",m_sFileName);
			return(0);
		} // end if

		CWOOD_LOG_DEBUG("Polygon %d:", iPolyIndex);

		// each vertex list MUST have 3 vertices since we made this a rule that all models
		// must be constructed of triangles
		// read in surface descriptor, number of vertices, and vertex list
		sscanf_s(pTokenString, "%s %s %d %d %d %d", 
			sTmpDescBuffer, sizeof(sTmpDescBuffer),
			sTmpColorBuffer, sizeof(sTmpColorBuffer),
			&iPolyNumVertexs, // should always be 3 
			&m_stPolyDataList[iPolyIndex].iVertexArray[0],
			&m_stPolyDataList[iPolyIndex].iVertexArray[1],
			&m_stPolyDataList[iPolyIndex].iVertexArray[2]);


		// since we are allowing the surface descriptor to be in hex format
		// with a leading "0x" we need to test for it
		if (sTmpDescBuffer[0] == '0' && toupper(sTmpDescBuffer[1]) == 'X'){
			sscanf_s(sTmpDescBuffer,"%x", &iPolySurfaceDesc);
		}
		else{
			iPolySurfaceDesc = atoi(sTmpDescBuffer);
		}

		if (sTmpColorBuffer[0] == '0' && toupper(sTmpColorBuffer[1]) == 'X'){
			sscanf_s(sTmpColorBuffer,"%x", &iPolyColor);
		}
		else{
			iPolyColor = atoi(sTmpColorBuffer);
		}

		// point polygon vertex list to object's vertex list
		// note that this is redundant since the polylist is contained
		// within the object in this case and its up to the user to select
		// whether the local or transformed vertex list is used when building up
		// polygon geometry, might be a better idea to set to NULL in the context
		// of polygons that are part of an object
		m_stPolyDataList[iPolyIndex].vlist = m_PointListLocal; 

		CWOOD_LOG_DEBUG("Surface Desc = 0x%.4x, ColorBuffer:%s Color:%x num_verts = %d, vert_indices [%d, %d, %d]", 
			iPolySurfaceDesc,
			sTmpColorBuffer,
			iPolyColor,
			iPolyNumVertexs, 
			m_stPolyDataList[iPolyIndex].iVertexArray[0],
			m_stPolyDataList[iPolyIndex].iVertexArray[1],
			m_stPolyDataList[iPolyIndex].iVertexArray[2]);

		// now we that we have the vertex list and we have entered the polygon
		// vertex index data into the polygon itself, now let's analyze the surface
		// descriptor and set the fields for the polygon based on the description

		// extract out each field of data from the surface descriptor
		// first let's get the single/double sided stuff out of the way
		if ((iPolySurfaceDesc & CW_PLX_2SIDED_FLAG)){
			SET_BIT(m_stPolyDataList[iPolyIndex].iAttr, CW_POLY4DV1_ATTR_2SIDED);
			CWOOD_LOG_DEBUG("2 sided.");
		} // end if
		else{
			// one sided
			CWOOD_LOG_DEBUG("1 sided.");
		} // end else

		// now let's set the color type and color
		// this is an RGB 4.4.4 surface
		SET_BIT(m_stPolyDataList[iPolyIndex].iAttr,CW_POLY4DV1_ATTR_RGB32);
		m_stPolyDataList[iPolyIndex].iColor = iPolyColor;
		CWOOD_LOG_DEBUG("RGB color = %d", m_stPolyDataList[iPolyIndex].iColor );
	


		// handle shading mode
		int shade_mode = (iPolySurfaceDesc & CW_PLX_SHADE_MODE_MASK);

		// set polygon shading mode
		switch(shade_mode)
		{
		case CW_PLX_SHADE_MODE_PURE_FLAG: {
			SET_BIT(m_stPolyDataList[iPolyIndex].iAttr, CW_POLY4DV1_ATTR_SHADE_MODE_PURE);
			CWOOD_LOG_DEBUG("Shade mode = pure");
									   } break;

		case CW_PLX_SHADE_MODE_FLAT_FLAG: {
			SET_BIT(m_stPolyDataList[iPolyIndex].iAttr, CW_POLY4DV1_ATTR_SHADE_MODE_FLAT);
			CWOOD_LOG_DEBUG("Shade mode = flat");

									   } break;

		case CW_PLX_SHADE_MODE_GOURAUD_FLAG: {
			SET_BIT(m_stPolyDataList[iPolyIndex].iAttr, CW_POLY4DV1_ATTR_SHADE_MODE_GOURAUD);
			CWOOD_LOG_DEBUG("Shade mode = gouraud");
										  } break;

		case CW_PLX_SHADE_MODE_PHONG_FLAG: {
			SET_BIT(m_stPolyDataList[iPolyIndex].iAttr, CW_POLY4DV1_ATTR_SHADE_MODE_PHONG);
			CWOOD_LOG_DEBUG("Shade mode = phong");
										} break;

		default: break;
		} // end switch

		// finally set the polygon to active
		m_stPolyDataList[iPolyIndex].iState = CW_POLY4DV1_STATE_ACTIVE;    

	} // end for poly



	for (int iVertexIndex = 0; iVertexIndex < m_iVerticeNum; iVertexIndex++)
	{
		// store result back
		VECTOR4D_COPY(&m_PointListTrans[iVertexIndex], &m_PointListLocal[iVertexIndex]);
	} // end for vertex


	// close the file
	fclose(fp);

	// return success
	return(1);

} // end Load_OBJECT4DV1_PLG

void CWObject::TransForm( MATRIX4X4 * pStMatrix,int iCoordType ){

	if(iCoordType == CW_TRANSFORM_LOCAL_ONLY){

		// all good, let's transform 
		for (int iVertexIndex = 0; iVertexIndex < m_iVerticeNum; iVertexIndex++)
		{
			// transform the vertex by mt
			POINT4D stPointRet; // hold result of each transformation

			// transform point
			CWMath::Matrix_X_Vector4D_4X4(&m_PointListLocal[iVertexIndex], pStMatrix, &stPointRet);

			// store result back
			VECTOR4D_COPY(&m_PointListLocal[iVertexIndex], &stPointRet); 
		} // end for vertex

	}
	else if(iCoordType == CW_TRANSFORM_TRANS_ONLY){
		// all good, let's transform 
		for (int iVertexIndex = 0; iVertexIndex < m_iVerticeNum; iVertexIndex++)
		{
			// transform the vertex by mt
			POINT4D stPointRet; // hold result of each transformation

			// transform point
			CWMath::Matrix_X_Vector4D_4X4(&m_PointListLocal[iVertexIndex], pStMatrix, &stPointRet);

			// store result back
			VECTOR4D_COPY(&m_PointListTrans[iVertexIndex], &stPointRet);
		} // end for vertex
	}
	else if(iCoordType == CW_TRANSFORM_LOCAL_TO_TRANS){

	}
}

