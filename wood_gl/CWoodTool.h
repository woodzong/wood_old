#ifndef CWOODTOOL_H_123897189712ADASDS
#define CWOODTOOL_H_123897189712ADASDS

#include "WoodFire_comm.h"


namespace wood{
	namespace comm{
		namespace tool{
			class CWoodTool{
			public:
				static bool IsRectCrossed( float fXA1,float fYA1, float fXA2, float fYA2,
					                       float fXB1,float fYB1, float fXB2, float fYB2 );

				static bool IsRectCrossed( ST_WOOD_RECT * pstRectA,ST_WOOD_RECT * pstRectB);
			};
		}
	}
}


#endif
