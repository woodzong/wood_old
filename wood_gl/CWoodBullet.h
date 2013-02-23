#ifndef __WOODFIRE_BULLET_COMM_H_ASDSDASDASDASDA____
#define __WOODFIRE_BULLET_COMM_H_ASDSDASDASDASDA____

#include "WoodFire_comm.h"

namespace wood{
	namespace fire{
		namespace bullet{

			class CWoodBullet{
			public:
				  CWoodBullet( GLfloat fX, GLfloat fY, GLfloat fSpeed, int iDirection, int iType, int iOwnerId);

				  ~CWoodBullet();

				  void ShowNow();

				  void SetSpeed( GLfloat fSpeed );

				  void GoOn();

				  void Stop();				  

				  bool IsKilled();

				  ST_WOOD_RECT * GetMyRect();

				  GLfloat GetOneWidth(){ return 0.15; };

				  int GetOwnerId(){ return m_iOwnerId; }

				  void Boom();

				  bool IsBoomed();

			private:
				void SetDirection( int iDirection );

			private:	
				GLuint  m_iUnitInCache;
				GLfloat m_fX; //current x cordinator 
				GLfloat m_fY; //current y cordinator
				GLfloat m_fSpeed;
				int m_iDirection;
				int m_iType;
				int m_iOwnerId;
				ST_WOOD_RECT m_stRect;
				int m_iBoom;
				int m_iBoomed;
			};
		}
	}
}

#endif