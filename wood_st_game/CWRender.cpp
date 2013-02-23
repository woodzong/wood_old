#include "CWRender.h"
#include "CWoodLog.h"

CWRender::CWRender():m_iState(0),m_iAttr(0),m_iPolyNum(0){
	memset(&m_pPolyArray,0,sizeof(m_pPolyArray));
	memset(&m_stPolyRendData,0,sizeof(m_stPolyRendData));
}

int CWRender::Reset(){
	m_iPolyNum = 0;
	return 0;
}

int CWRender::AddPoly4Data( const ST_POLY4DATA & rStPoly ){
	if(m_iPolyNum>=CW_RENDERLIST4DV1_MAX_POLYS){
		return -1;
	}
	
	m_pPolyArray[m_iPolyNum] = &m_stPolyRendData[m_iPolyNum];

	//---- start convert Poly4Data Into Poly4Rend

	m_stPolyRendData[m_iPolyNum].iState = rStPoly.iState;
	m_stPolyRendData[m_iPolyNum].iAttr  = rStPoly.iAttr;
	m_stPolyRendData[m_iPolyNum].iColor = rStPoly.iColor;
	
	VECTOR4D_COPY(&m_stPolyRendData[m_iPolyNum].tvlist[0],&rStPoly.vlist[rStPoly.iVertexArray[0]]);
	VECTOR4D_COPY(&m_stPolyRendData[m_iPolyNum].tvlist[1],&rStPoly.vlist[rStPoly.iVertexArray[1]]);
	VECTOR4D_COPY(&m_stPolyRendData[m_iPolyNum].tvlist[2],&rStPoly.vlist[rStPoly.iVertexArray[2]]);

	VECTOR4D_COPY(&m_stPolyRendData[m_iPolyNum].vlist[0],&rStPoly.vlist[rStPoly.iVertexArray[0]]);
	VECTOR4D_COPY(&m_stPolyRendData[m_iPolyNum].vlist[1],&rStPoly.vlist[rStPoly.iVertexArray[1]]);
	VECTOR4D_COPY(&m_stPolyRendData[m_iPolyNum].vlist[2],&rStPoly.vlist[rStPoly.iVertexArray[2]]);

	//--- convert end...
	
	// now the polygon is loaded into the next free array position, but
	// we need to fix up the links
	// test if this is the first entry
	if ( m_iPolyNum == 0){
		// set pointers to null, could loop them around though to self
		m_stPolyRendData[0].pNext = NULL;
		m_stPolyRendData[0].pPrev = NULL;
	} // end if
	else{
		// first set this node to point to previous node and next node (null)
		m_stPolyRendData[m_iPolyNum].pNext = NULL;
		m_stPolyRendData[m_iPolyNum].pPrev = &m_stPolyRendData[m_iPolyNum-1];

		// now set previous node to point to this node
		m_stPolyRendData[m_iPolyNum-1].pNext = &m_stPolyRendData[m_iPolyNum];
	} // end else

	m_iPolyNum++;
	
	return m_iPolyNum;
}


int CWRender::AddPoly4Rend(const ST_POLY4REND & rStPoly){
	if(m_iPolyNum>=CW_RENDERLIST4DV1_MAX_POLYS){
		return -1;
	}

	// step 1: copy polygon into next opening in polygon render list

	// point pointer to polygon structure
	m_pPolyArray[m_iPolyNum] = &m_stPolyRendData[m_iPolyNum];


	CWOOD_LOG_DEBUG("RendItOut Now!! m_iPolyNum:%d m_pPolyArray:%d",m_pPolyArray[m_iPolyNum] );

	// copy face right into array, thats it
	memcpy((void *)&m_stPolyRendData[m_iPolyNum],(void *)&rStPoly, sizeof(ST_POLY4REND));

	// now the polygon is loaded into the next free array position, but
	// we need to fix up the links
	// test if this is the first entry
	if ( m_iPolyNum == 0){
		// set pointers to null, could loop them around though to self
		m_stPolyRendData[0].pNext = NULL;
		m_stPolyRendData[0].pPrev = NULL;
	} // end if
	else{
		// first set this node to point to previous node and next node (null)
		m_stPolyRendData[m_iPolyNum].pNext = NULL;
		m_stPolyRendData[m_iPolyNum].pPrev = &m_stPolyRendData[m_iPolyNum-1];

		// now set previous node to point to this node
		m_stPolyRendData[m_iPolyNum-1].pNext = &m_stPolyRendData[m_iPolyNum];
	} // end else

	m_iPolyNum++;


	return m_iPolyNum;
}


int CWRender::AddObject(const CWObject & rWObject, int iInsertLocal){
	if( m_iPolyNum + rWObject.GetPolyNum() >= CW_RENDERLIST4DV1_MAX_POLYS ){
		return -1;
	}

	int iRetCode = 0;

	if (!(rWObject.GetState() & CW_OBJECT4DV1_STATE_ACTIVE) ||
		(rWObject.GetState() & CW_OBJECT4DV1_STATE_CULLED) ||
		!(rWObject.GetState() & CW_OBJECT4DV1_STATE_VISIBLE))
		return 0; 

	for(int iIndex = 0; iIndex<rWObject.GetPolyNum(); iIndex ++){
		ST_POLY4DATA stPolyDataTmp; 
		memcpy( &stPolyDataTmp,rWObject.GetPolyPointer(iIndex), sizeof(ST_POLY4DATA));

		if (!(stPolyDataTmp.iState & CW_POLY4DV1_STATE_ACTIVE  ) ||
			 (stPolyDataTmp.iState & CW_POLY4DV1_STATE_CLIPPED ) ||
			 (stPolyDataTmp.iState & CW_POLY4DV1_STATE_BACKFACE) ){
			continue; // move onto next poly
		}

		if(iInsertLocal){
			stPolyDataTmp.vlist = ( POINT4D *) rWObject.GetLocalPointList();
		}
		else{
			stPolyDataTmp.vlist = ( POINT4D *) rWObject.GetTransPointList();
		}
		
		iRetCode = AddPoly4Data( stPolyDataTmp );
		if(iRetCode <= 0){
			CWOOD_LOG_ERR_L(iRetCode,"AddPoly4Data Failed pPolyData:%d",&stPolyDataTmp);	
			return -1;
		}
		//AddPoly( *rWObject.GetPolyPointer(iIndex) );
	}

	return m_iPolyNum;
}


int CWRender::Transform(MATRIX4X4 * pStMatrix,int iCoordType){

	if(iCoordType == CW_TRANSFORM_LOCAL_ONLY){

		for(int iIndex = 0; iIndex<m_iPolyNum; iIndex ++){
			ST_POLY4REND * pPoly4Rend = m_pPolyArray[iIndex];
			if ((pPoly4Rend==NULL) || !(pPoly4Rend->iState & CW_POLY4DV1_STATE_ACTIVE) ||
				(pPoly4Rend->iState & CW_POLY4DV1_STATE_CLIPPED ) ||
				(pPoly4Rend->iState & CW_POLY4DV1_STATE_BACKFACE) ){
					continue; // move onto next poly
			}

			// all good, let's transform 
			for (int iVertexIndex = 0; iVertexIndex < 3; iVertexIndex++)
			{
				// transform the vertex by mt
				POINT4D stPointRet; // hold result of each transformation

				// transform point
				CWMath::Matrix_X_Vector4D_4X4(&pPoly4Rend->vlist[iVertexIndex], pStMatrix, &stPointRet);

				// store result back
				VECTOR4D_COPY(&pPoly4Rend->vlist[iVertexIndex], &stPointRet); 
			} // end for vertex
		}

	}
	else if(iCoordType == CW_TRANSFORM_TRANS_ONLY){

	}
	else if(iCoordType == CW_TRANSFORM_LOCAL_TO_TRANS){

	}

	return 0;
}


int CWRender::ModelToWorld(POINT4D * pStPointWordPos,int iCoordType){
	// NOTE: Not matrix based
	// this function converts the local model coordinates of the
	// sent render list into world coordinates, the results are stored
	// in the transformed vertex list (tvlist) within the renderlist

	// interate thru vertex list and transform all the model/local 
	// coords to world coords by translating the vertex list by
	// the amount world_pos and storing the results in tvlist[]
	// is this polygon valid?
	for(int iIndex = 0; iIndex<m_iPolyNum; iIndex ++){
		ST_POLY4REND * pPoly4Rend = m_pPolyArray[iIndex];
		if ((pPoly4Rend==NULL) || !(pPoly4Rend->iState & CW_POLY4DV1_STATE_ACTIVE) ||
			(pPoly4Rend->iState & CW_POLY4DV1_STATE_CLIPPED ) ||
			(pPoly4Rend->iState & CW_POLY4DV1_STATE_BACKFACE) ){
				continue; // move onto next poly
		}

		// all good, let's transform 
		for (int iVertexIndex = 0; iVertexIndex < 3; iVertexIndex++)
		{
			// store result back
			if (iCoordType == CW_TRANSFORM_LOCAL_TO_TRANS){	
				CWMath::Vector4D_ADD(&pPoly4Rend->vlist[iVertexIndex], pStPointWordPos, &pPoly4Rend->tvlist[iVertexIndex]);
			}
			else{
				CWMath::Vector4D_ADD(&pPoly4Rend->tvlist[iVertexIndex], pStPointWordPos, &pPoly4Rend->tvlist[iVertexIndex]);
			}
		} // end for vertex
	}

	return 0;
}


int CWRender::WorldToCamera( CWCamera * pWCamera ){

	// NOTE: this is a matrix based function
	// this function transforms each polygon in the global render list
	// to camera coordinates based on the sent camera transform matrix
	// you would use this function instead of the object based function
	// if you decided earlier in the pipeline to turn each object into 
	// a list of polygons and then add them to the global render list
	// the conversion of an object into polygons probably would have
	// happened after object culling, local transforms, local to world
	// and backface culling, so the minimum number of polygons from
	// each object are in the list, note that the function assumes
	// that at LEAST the local to world transform has been called
	// and the polygon data is in the transformed list tvlist of
	// the POLYF4DV1 object

	// transform each polygon in the render list into camera coordinates
	// assumes the render list has already been transformed to world
	// coordinates and the result is in tvlist[] of each polygon object

	for(int iIndex = 0; iIndex<m_iPolyNum; iIndex ++){
		ST_POLY4REND * pPoly4Rend = m_pPolyArray[iIndex];

		if ((pPoly4Rend==NULL) || !(pPoly4Rend->iState & CW_POLY4DV1_STATE_ACTIVE) ||
			(pPoly4Rend->iState & CW_POLY4DV1_STATE_CLIPPED ) ||
			(pPoly4Rend->iState & CW_POLY4DV1_STATE_BACKFACE) ){
				continue; // move onto next poly
		}

		// all good, let's transform 
		for (int iVertexIndex = 0; iVertexIndex < 3; iVertexIndex++)
		{
			// transform the vertex by mt
			POINT4D stPointRet; // hold result of each transformation

			// transform point
			CWMath::Matrix_X_Vector4D_4X4(&pPoly4Rend->tvlist[iVertexIndex], &pWCamera->GetCameraDataPointer()->stMatrixCam, &stPointRet);

			// store result back
			VECTOR4D_COPY(&pPoly4Rend->tvlist[iVertexIndex], &stPointRet); 
		} // end for vertex
	}
	return 0;
}

int CWRender::CameraToPerspective( CWCamera * pWCamera ){

	// NOTE: this is not a matrix based function
	// this function transforms each polygon in the global render list
	// into perspective coordinates, based on the 
	// sent camera object, 
	// you would use this function instead of the object based function
	// if you decided earlier in the pipeline to turn each object into 
	// a list of polygons and then add them to the global render list

	// transform each polygon in the render list into camera coordinates
	// assumes the render list has already been transformed to world
	// coordinates and the result is in tvlist[] of each polygon object

	for(int iIndex = 0; iIndex<m_iPolyNum; iIndex ++){
		ST_POLY4REND * pPoly4Rend = m_pPolyArray[iIndex];

		if ((pPoly4Rend==NULL) || !(pPoly4Rend->iState & CW_POLY4DV1_STATE_ACTIVE) ||
			(pPoly4Rend->iState & CW_POLY4DV1_STATE_CLIPPED ) ||
			(pPoly4Rend->iState & CW_POLY4DV1_STATE_BACKFACE) ){
				continue; // move onto next poly
		}

		// all good, let's transform 
		for (int iVertexIndex = 0; iVertexIndex < 3; iVertexIndex++)
		{
			// transform the vertex by mt
			float z = pPoly4Rend->tvlist[iVertexIndex].z;
			pPoly4Rend->tvlist[iVertexIndex].x = pWCamera->GetCameraDataPointer()->fViewDistance * pPoly4Rend->tvlist[iVertexIndex].x / z;
			pPoly4Rend->tvlist[iVertexIndex].y = pWCamera->GetCameraDataPointer()->fViewDistance * pPoly4Rend->tvlist[iVertexIndex].y / z;

		} // end for vertex
	}

	return 0;
}


int CWRender::PerspectiveToScreen( CWCamera * pWCamera  ){
	// NOTE: this is not a matrix based function
	// this function transforms the perspective coordinates of the render
	// list into screen coordinates, based on the sent viewport in the camera
	// assuming that the viewplane coordinates were normalized
	// you would use this function instead of the object based function
	// if you decided earlier in the pipeline to turn each object into 
	// a list of polygons and then add them to the global render list
	// you would only call this function if you previously performed
	// a normalized perspective transform

	// transform each polygon in the render list from perspective to screen 
	// coordinates assumes the render list has already been transformed 
	// to normalized perspective coordinates and the result is in tvlist[]	


	for(int iIndex = 0; iIndex<m_iPolyNum; iIndex ++){
		ST_POLY4REND * pPoly4Rend = m_pPolyArray[iIndex];

		if ((pPoly4Rend==NULL) || !(pPoly4Rend->iState & CW_POLY4DV1_STATE_ACTIVE) ||
			(pPoly4Rend->iState & CW_POLY4DV1_STATE_CLIPPED ) ||
			(pPoly4Rend->iState & CW_POLY4DV1_STATE_BACKFACE) ){
				continue; // move onto next poly
		}

		float alpha = (float)(0.5 * pWCamera->GetCameraDataPointer()->fViewPortWidth  - 0.5);
		float beta  = (float)(0.5 * pWCamera->GetCameraDataPointer()->fViewPortHeight - 0.5);

		// all good, let's transform 
		for (int iVertexIndex = 0; iVertexIndex < 3; iVertexIndex++)
		{
			pPoly4Rend->tvlist[iVertexIndex].x = alpha + alpha * pPoly4Rend->tvlist[iVertexIndex].x;
			pPoly4Rend->tvlist[iVertexIndex].y = beta  - beta  * pPoly4Rend->tvlist[iVertexIndex].y;

		} // end for vertex
	}
	return 0;
}


int CWRender::RendItOut( CWDraw & rWDraw ){
	
	//CWOOD_LOG_DEBUG("RendItOut Now!! m_iPolyNum:%d",m_iPolyNum);

	for(int iIndex = 0; iIndex<m_iPolyNum; iIndex ++){
		ST_POLY4REND * pPoly4Rend = m_pPolyArray[iIndex];

		//CWOOD_LOG_DEBUG("pPoly4Rend:%d pPoly4Rend:%d",pPoly4Rend,pPoly4Rend);

		//CWOOD_LOG_DEBUG("m_iPolyNum:%d  pPoly4Rend->iState:%d  (pPoly4Rend->iState & CW_POLY4DV1_STATE_ACTIVE):%d",
		//	             m_iPolyNum,pPoly4Rend->iState,(pPoly4Rend->iState & CW_POLY4DV1_STATE_ACTIVE) );

		if ((pPoly4Rend==NULL) || !(pPoly4Rend->iState & CW_POLY4DV1_STATE_ACTIVE) ||
			(pPoly4Rend->iState & CW_POLY4DV1_STATE_CLIPPED ) ||
			(pPoly4Rend->iState & CW_POLY4DV1_STATE_BACKFACE) ){
				continue; // move onto next poly
		}

		rWDraw.DrawClipLine32Bit(pPoly4Rend->tvlist[0].x, 
			pPoly4Rend->tvlist[0].y,
			pPoly4Rend->tvlist[1].x, 
			pPoly4Rend->tvlist[1].y,
			pPoly4Rend->iColor);

		rWDraw.DrawClipLine32Bit(pPoly4Rend->tvlist[1].x, 
			pPoly4Rend->tvlist[1].y,
			pPoly4Rend->tvlist[2].x, 
			pPoly4Rend->tvlist[2].y,
			pPoly4Rend->iColor);

		rWDraw.DrawClipLine32Bit(pPoly4Rend->tvlist[2].x, 
			pPoly4Rend->tvlist[2].y,
			pPoly4Rend->tvlist[0].x, 
			pPoly4Rend->tvlist[0].y,
			pPoly4Rend->iColor);
	}

	return 0;
}