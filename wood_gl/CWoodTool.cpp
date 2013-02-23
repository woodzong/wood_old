#include <algorithm>

#include "CWoodTool.h"

using namespace std;
using namespace wood::comm::tool;

//if RectA is in Rect B, it is also return true;
bool CWoodTool::IsRectCrossed( float fX_A_LeftDown,float fY_A_LeftDown, float fX_A_RightUp, float fY_A_RightUp,
							   float fX_B_LeftDown,float fY_B_LeftDown, float fX_B_RightUp, float fY_B_RightUp ){
	float fXTA1 = max(fX_A_LeftDown,fX_B_LeftDown);
	float fXTA2 = min(fX_A_RightUp,fX_B_RightUp);
	if( fXTA1 >= fXTA2 ){
		return false;
	}

	float fYTA1 = max(fY_A_LeftDown,fY_B_LeftDown);
	float fYTA2 = min(fY_A_RightUp,fY_B_RightUp);
	if( fYTA1 >= fYTA2 ){
		return false;
	}

	return true;
}

bool CWoodTool::IsRectCrossed( ST_WOOD_RECT * pstRectA,ST_WOOD_RECT * pstRectB){
	return CWoodTool::IsRectCrossed( pstRectA->fXLeftDown, pstRectA->fYLeftDown, pstRectA->fXRightUp, pstRectA->fYRightUp, 
									 pstRectB->fXLeftDown, pstRectB->fYLeftDown, pstRectB->fXRightUp, pstRectB->fYRightUp );
}
