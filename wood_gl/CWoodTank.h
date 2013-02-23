#ifndef __WOODFIRE_TANK_COMM_H_ASDSDASDASDASDA____
#define __WOODFIRE_TANK_COMM_H_ASDSDASDASDASDA____

#include "WoodFire_comm.h"
#include "CWoodBullet.h"

namespace wood{
	namespace fire{
		namespace tank{

			class CWoodTank{
			public:
				  CWoodTank( GLfloat fX, GLfloat fY, GLfloat fSpeed, int iDirection, int iId);

				  virtual ~CWoodTank();

				  void ShowNow();

				  void SetSpeed( GLfloat fSpeed );

				  void Stop();
				  void GoUp();
				  void GoDown();
				  void GoLeft();
				  void GoRight();	
				  void GoStraight();
				  void GoOpposite();
				  void RollBack( GLfloat fDistance ); //return to the presistion;
				  void BeHit();
				  void BeFired();
				  void SetTankType(int iTankType);
				  int GetId(){ return m_iId;};
				  virtual void AutoAction(wood::fire::bullet::CWoodBullet * &pWoodBullet){};

				  ST_WOOD_RECT * GetMyRect();
				  ST_WOOD_RECT * GetMyPreRect();

				  wood::fire::bullet::CWoodBullet * Fire(  wood::fire::bullet::CWoodBullet * &pWoodBullet );

				  int GetCurrentDirection();
				  GLfloat GetCurrentSpeed();
				  GLfloat GetX(){return m_fX;}
				  GLfloat GetY(){return m_fY;}
				  GLfloat GetPreX(){ return m_fPreX; };
				  GLfloat GetPreY(){ return m_fPreY; };
				  GLfloat GetPreDirection(){ return m_iPreDirection; };
				  GLfloat GetOneWidth(){ return 1.0/2; };

				  void HVPreCloseTo(CWoodTank * pTankTarget ); // //H for horizontal, V for Vertical	

			protected:
				void SetDirection( int iDirection );
				void FixtoBorderLine();
				void FixToXCollistion( GLfloat fX );
				void FixToYCollistion( GLfloat fY );
				bool IsReachedBorderline();
				

			protected:	
				GLuint  m_iUnitInCache;
				GLfloat m_fX; //current x cordinator 
				GLfloat m_fY; //current y cordinator
				GLfloat m_fSpeed;
				int m_iId;
				int m_iDirection;				
				int m_iTurnCount;
				GLfloat m_fMovedDistance;
				GLfloat m_fFiredRef;				
				GLfloat m_fPreX; //pre x cordinator;
				GLfloat m_fPreY; //pre y cordinator;
				int m_iPreDirection;					
				GLfloat m_fCurrentSpeed;
				GLfloat m_fColor[3];
				int m_iIsHit;
				int m_iIsFired;
				int m_iTankType;

			private:
				ST_WOOD_RECT m_stRect;
				ST_WOOD_RECT m_stPreRect;
			};


			class CWoodTankCom: public CWoodTank{
			public:
				CWoodTankCom( GLfloat fX, GLfloat fY, GLfloat fSpeed, int iDirection,int iId)
					:CWoodTank(fX, fY, fSpeed,iDirection,iId)
				{
					m_iTankType = DWOOD_TANK_TYPE_ENEMY;
				};
						
				void AutoAction( wood::fire::bullet::CWoodBullet * &pWoodBullet );

			private:

			};
		}
	}
}

#endif