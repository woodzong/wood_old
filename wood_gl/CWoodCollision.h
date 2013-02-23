#ifndef CWOODCOLLISION_H_576514510
#define CWOODCOLLISION_H_576514510

#include <vector>
#include "CWoodTank.h"
#include "CWoodTool.h"
#include "CWoodBullet.h"

namespace wood{
	namespace comm{
		namespace collision{
			class CWoodCollision{
			public:
				CWoodCollision(){};

				void HandelCollision(std::vector<wood::fire::tank::CWoodTank *> & vecWoodEnemy);

				void HandelBullet( std::vector<wood::fire::tank::CWoodTank *> & vecAllTank,
					               std::vector<wood::fire::bullet::CWoodBullet *> & vecAllBullet );

			private:
				int GetCollisionType( wood::fire::tank::CWoodTank * pTankA, wood::fire::tank::CWoodTank * pTankB);
				void DoCollisionWithType(wood::fire::tank::CWoodTank * pTankA, wood::fire::tank::CWoodTank * pTankB,int iCollisionType);

				void DoOpposite(wood::fire::tank::CWoodTank * pTankA, wood::fire::tank::CWoodTank * pTankB);
				void DoKiss(wood::fire::tank::CWoodTank * pTankA, wood::fire::tank::CWoodTank * pTankB);
				void DoCross(wood::fire::tank::CWoodTank * pTankA, wood::fire::tank::CWoodTank * pTankB);

				wood::fire::tank::CWoodTank * GetMinSpeedTank(wood::fire::tank::CWoodTank * pTankA, wood::fire::tank::CWoodTank * pTankB);
				wood::fire::tank::CWoodTank * GetMaxSpeedTank(wood::fire::tank::CWoodTank * pTankA, wood::fire::tank::CWoodTank * pTankB);
			};
		}
	}
}

#endif