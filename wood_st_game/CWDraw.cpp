#include "CWDraw.h"
#include "CWoodLog.h"

CWDraw::CWDraw(IDirect3DSurface9* _pSurface):
m_pSurface(_pSurface),
m_iPitch32(0),
m_iPitch16(0),
m_pData(0),
m_bIsReleased(true),
m_iScreenWidth(0),
m_iScreenHeight(0){
	Init();
}

CWDraw::~CWDraw(){
	Release();
}

void CWDraw::Init(){
	LONG lRetCode = 0;
	D3DSURFACE_DESC stSurfaceDesc;
	lRetCode = m_pSurface->GetDesc(&stSurfaceDesc);
	if(lRetCode != D3D_OK){
		CWOOD_LOG_ERR_L(lRetCode,"m_pSurface->GetDesc Failed, pSurface:%d",m_pSurface);
		return;
	}

	D3DLOCKED_RECT stLockedRect;
	lRetCode = m_pSurface->LockRect(&stLockedRect,0,0);
	if(lRetCode != D3D_OK){
		CWOOD_LOG_ERR_L(lRetCode,"m_pSurface->LockRect Failed, D3DERR_INVALIDCALL:%lu  D3DERR_WASSTILLDRAWING:%lu pSurface:%d",
			m_pSurface,D3DERR_INVALIDCALL,D3DERR_WASSTILLDRAWING);
		return;
	}
	m_iPitch32 = stLockedRect.Pitch >> 2;
	m_iPitch16 = stLockedRect.Pitch >> 1;
	m_pData = (DWORD *)stLockedRect.pBits;	
	m_iScreenWidth  = stSurfaceDesc.Width;
	m_iScreenHeight = stSurfaceDesc.Height;
	m_bIsReleased = false;
	return;
}

void CWDraw::Release(){
	if(m_bIsReleased){
		return;
	}
	LONG lRetCode = m_pSurface->UnlockRect();
	if( lRetCode != D3D_OK ){
		CWOOD_LOG_ERR_L(lRetCode,"pSurface->UnlockRect Failed, pSurface:%d D3DERR_INVALIDCALL:%ld",m_pSurface,D3DERR_INVALIDCALL);
		return;
	}
	m_bIsReleased = true;
	return;
}

int CWDraw::ClipLine(int &x1,int &y1,int &x2, int &y2){
	// this function clips the sent line using the globally defined clipping
	// region
	int min_clip_x = 0,                             // clipping rectangle 
		max_clip_x = (m_iScreenWidth-1),
		min_clip_y = 0,
		max_clip_y = (m_iScreenHeight-1);


	int xc1=x1, 
		yc1=y1, 
		xc2=x2, 
		yc2=y2;

	int p1_code=0, 
		p2_code=0;

	// determine codes for p1 and p2
	if (y1 < min_clip_y)
		p1_code|=CLIP_CODE_N;
	else
		if (y1 > max_clip_y)
			p1_code|=CLIP_CODE_S;

	if (x1 < min_clip_x)
		p1_code|=CLIP_CODE_W;
	else
		if (x1 > max_clip_x)
			p1_code|=CLIP_CODE_E;

	if (y2 < min_clip_y)
		p2_code|=CLIP_CODE_N;
	else
		if (y2 > max_clip_y)
			p2_code|=CLIP_CODE_S;

	if (x2 < min_clip_x)
		p2_code|=CLIP_CODE_W;
	else
		if (x2 > max_clip_x)
			p2_code|=CLIP_CODE_E;

	// try and trivially reject
	if ((p1_code & p2_code)) 
		return(0);

	// test for totally visible, if so leave points untouched
	if (p1_code==0 && p2_code==0)
		return(1);

	// determine end clip point for p1
	switch(p1_code)
	{
	case CLIP_CODE_C: break;

	case CLIP_CODE_N:
		{
			yc1 = min_clip_y;
			xc1 = (int)(x1 + 0.5+(min_clip_y-y1)*(x2-x1)/(y2-y1)); //根据斜率,计算屏幕边缘直线的点的x坐标. wood
		} break;
	case CLIP_CODE_S:
		{
			yc1 = max_clip_y;
			xc1 = (int)(x1 + 0.5+(max_clip_y-y1)*(x2-x1)/(y2-y1));
		} break;

	case CLIP_CODE_W:
		{
			xc1 = min_clip_x;
			yc1 = (int)(y1 + 0.5+(min_clip_x-x1)*(y2-y1)/(x2-x1));
		} break;

	case CLIP_CODE_E:
		{
			xc1 = max_clip_x;
			yc1 = (int)(y1 + 0.5+(max_clip_x-x1)*(y2-y1)/(x2-x1));
		} break;

		// these cases are more complex, must compute 2 intersections
	case CLIP_CODE_NE:
		{
			// north hline intersection
			yc1 = min_clip_y;
			xc1 = (int)(x1 + 0.5+(min_clip_y-y1)*(x2-x1)/(y2-y1));

			// test if intersection is valid, of so then done, else compute next
			if (xc1 < min_clip_x || xc1 > max_clip_x)
			{
				// east vline intersection
				xc1 = max_clip_x;
				yc1 = (int)(y1 + 0.5+(max_clip_x-x1)*(y2-y1)/(x2-x1));
			} // end if

		} break;

	case CLIP_CODE_SE:
		{
			// south hline intersection
			yc1 = max_clip_y;
			xc1 = (int)(x1 + 0.5+(max_clip_y-y1)*(x2-x1)/(y2-y1));	

			// test if intersection is valid, of so then done, else compute next
			if (xc1 < min_clip_x || xc1 > max_clip_x)
			{
				// east vline intersection
				xc1 = max_clip_x;
				yc1 = (int)(y1 + 0.5+(max_clip_x-x1)*(y2-y1)/(x2-x1));
			} // end if

		} break;

	case CLIP_CODE_NW: 
		{
			// north hline intersection
			yc1 = min_clip_y;
			xc1 = (int)(x1 + 0.5+(min_clip_y-y1)*(x2-x1)/(y2-y1));

			// test if intersection is valid, of so then done, else compute next
			if (xc1 < min_clip_x || xc1 > max_clip_x)
			{
				xc1 = min_clip_x;
				yc1 = (int)(y1 + 0.5+(min_clip_x-x1)*(y2-y1)/(x2-x1));	
			} // end if

		} break;

	case CLIP_CODE_SW:
		{
			// south hline intersection
			yc1 = max_clip_y;
			xc1 = (int)(x1 + 0.5+(max_clip_y-y1)*(x2-x1)/(y2-y1));	

			// test if intersection is valid, of so then done, else compute next
			if (xc1 < min_clip_x || xc1 > max_clip_x)
			{
				xc1 = min_clip_x;
				yc1 = (int)(y1 + 0.5+(min_clip_x-x1)*(y2-y1)/(x2-x1));	
			} // end if

		} break;

	default:break;

	} // end switch

	// determine clip point for p2
	switch(p2_code)
	{
	case CLIP_CODE_C: break;

	case CLIP_CODE_N:
		{
			yc2 = min_clip_y;
			xc2 = (int)(x2 + (min_clip_y-y2)*(x1-x2)/(y1-y2));
		} break;

	case CLIP_CODE_S:
		{
			yc2 = max_clip_y;
			xc2 = (int)(x2 + (max_clip_y-y2)*(x1-x2)/(y1-y2));
		} break;

	case CLIP_CODE_W:
		{
			xc2 = min_clip_x;
			yc2 = (int)(y2 + (min_clip_x-x2)*(y1-y2)/(x1-x2));
		} break;

	case CLIP_CODE_E:
		{
			xc2 = max_clip_x;
			yc2 = (int)(y2 + (max_clip_x-x2)*(y1-y2)/(x1-x2));
		} break;

		// these cases are more complex, must compute 2 intersections
	case CLIP_CODE_NE:
		{
			// north hline intersection
			yc2 = min_clip_y;
			xc2 = (int)(x2 + 0.5+(min_clip_y-y2)*(x1-x2)/(y1-y2));

			// test if intersection is valid, of so then done, else compute next
			if (xc2 < min_clip_x || xc2 > max_clip_x)
			{
				// east vline intersection
				xc2 = max_clip_x;
				yc2 = (int)(y2 + 0.5+(max_clip_x-x2)*(y1-y2)/(x1-x2));
			} // end if

		} break;

	case CLIP_CODE_SE:
		{
			// south hline intersection
			yc2 = max_clip_y;
			xc2 = (int)(x2 + 0.5+(max_clip_y-y2)*(x1-x2)/(y1-y2));	

			// test if intersection is valid, of so then done, else compute next
			if (xc2 < min_clip_x || xc2 > max_clip_x)
			{
				// east vline intersection
				xc2 = max_clip_x;
				yc2 = (int)(y2 + 0.5+(max_clip_x-x2)*(y1-y2)/(x1-x2));
			} // end if

		} break;

	case CLIP_CODE_NW: 
		{
			// north hline intersection
			yc2 = min_clip_y;
			xc2 = (int)(x2 + 0.5+(min_clip_y-y2)*(x1-x2)/(y1-y2));

			// test if intersection is valid, of so then done, else compute next
			if (xc2 < min_clip_x || xc2 > max_clip_x)
			{
				xc2 = min_clip_x;
				yc2 = (int)(y2 + 0.5+(min_clip_x-x2)*(y1-y2)/(x1-x2));	
			} // end if

		} break;

	case CLIP_CODE_SW:
		{
			// south hline intersection
			yc2 = max_clip_y;
			xc2 = (int)(x2 + 0.5+(max_clip_y-y2)*(x1-x2)/(y1-y2));	

			// test if intersection is valid, of so then done, else compute next
			if (xc2 < min_clip_x || xc2 > max_clip_x)
			{
				xc2 = min_clip_x;
				yc2 = (int)(y2 + 0.5+(min_clip_x-x2)*(y1-y2)/(x1-x2));	
			} // end if

		} break;

	default:break;

	} // end switch

	// do bounds check
	if ((xc1 < min_clip_x) || (xc1 > max_clip_x) ||
		(yc1 < min_clip_y) || (yc1 > max_clip_y) ||
		(xc2 < min_clip_x) || (xc2 > max_clip_x) ||
		(yc2 < min_clip_y) || (yc2 > max_clip_y) )
	{
		return(0);
	} // end if

	// store vars back
	x1 = xc1;
	y1 = yc1;
	x2 = xc2;
	y2 = yc2;

	return(1);

} // end Clip_Line


void CWDraw::DrawLine32Bit(int x0, int y0, // starting position 
						   int x1, int y1, // ending position
						   DWORD dwColor)     // color index
{
	// this function draws a line from xo,yo to x1,y1 using differential error
	// terms (based on Bresenahams work)
	if(m_bIsReleased){
		return;
	}

	int dx,             // difference in x's
		dy,             // difference in y's
		dx2,            // dx,dy * 2
		dy2, 
		x_inc,          // amount in pixel space to move during drawing
		y_inc,          // amount in pixel space to move during drawing
		error,          // the discriminant i.e. error i.e. decision variable
		index;          // used for looping

	// pre-compute first pixel address in video buffer based on 16bit data
	DWORD *pBuffer2 = m_pData + x0 + y0*m_iPitch32;

	// compute horizontal and vertical deltas
	dx = x1-x0;
	dy = y1-y0;

	// test which direction the line is going in i.e. slope angle
	if (dx>=0)
	{
		x_inc = 1;

	} // end if line is moving right
	else
	{
		x_inc = -1;
		dx    = -dx;  // need absolute value

	} // end else moving left

	// test y component of slope

	if (dy>=0)
	{
		y_inc = m_iPitch32;
	} // end if line is moving down
	else
	{
		y_inc = -m_iPitch32;
		dy    = -dy;  // need absolute value

	} // end else moving up

	// compute (dx,dy) * 2
	dx2 = dx << 1;
	dy2 = dy << 1;

	// now based on which delta is greater we can draw the line
	if (dx > dy)
	{
		// initialize error term
		error = dy2 - dx; 

		// draw the line
		for (index=0; index <= dx; index++)
		{
			// set the pixel
			*pBuffer2 = dwColor;

			// test if error has overflowed
			if (error >= 0) 
			{
				error-=dx2;

				// move to next line
				pBuffer2+=y_inc;

			} // end if error overflowed

			// adjust the error term
			error+=dy2;

			// move to the next pixel
			pBuffer2+=x_inc;

		} // end for

	} // end if |slope| <= 1
	else
	{
		// initialize error term
		error = dx2 - dy; 

		// draw the line
		for (index=0; index <= dy; index++)
		{
			// set the pixel
			*pBuffer2 = dwColor;

			// test if error overflowed
			if (error >= 0)
			{
				error-=dy2;

				// move to next line
				pBuffer2+=x_inc;

			} // end if error overflowed

			// adjust the error term
			error+=dx2;

			// move to the next pixel
			pBuffer2+=y_inc;

		} // end for

	} // end else |slope| > 1

	// return success
	return;

} // end DrawLine32Bit


void CWDraw::DrawClipLine32Bit(int x0,int y0, int x1, int y1, DWORD dwColor)
{
	// this function draws a clipped line
	// clip and draw each line
	int cxs = x0;
	int cys = y0;
	int cxe = x1;
	int cye = y1;

	//CWOOD_LOG_DEBUG("x0:%d y0:%d x1:%d y1:%d",x0,y0,x1,y1);

	// clip the line
	if (ClipLine(cxs,cys,cxe,cye)){
		DrawLine32Bit(cxs, cys, cxe,cye,dwColor);
	}

	return ;

} // end DrawClipLine32Bit
