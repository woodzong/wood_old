#include <math.h>
#include <float.h>
#include "CWoodCollision.h"

using namespace wood::comm::collision;
using namespace wood::fire::tank;
using namespace wood::fire::bullet;
using namespace wood::comm::tool;


wood::fire::tank::CWoodTank * CWoodCollision::GetMinSpeedTank(wood::fire::tank::CWoodTank * pTankA, wood::fire::tank::CWoodTank * pTankB){
	if( pTankA->GetCurrentSpeed() >= pTankB->GetCurrentSpeed() ){
		return pTankB;
	}
	return pTankA;
}


wood::fire::tank::CWoodTank * CWoodCollision::GetMaxSpeedTank(wood::fire::tank::CWoodTank * pTankA, wood::fire::tank::CWoodTank * pTankB){
	if( pTankA->GetCurrentSpeed() <= pTankB->GetCurrentSpeed() ){
		return pTankB;
	}
	return pTankA;
}


int CWoodCollision::GetCollisionType(CWoodTank * pTankA, CWoodTank * pTankB){
	if( pTankA->GetCurrentDirection() == pTankB->GetCurrentDirection() ){
		return DWOOD_COLLISION_DIRECTION_SAME;
	}
	else if( abs( pTankA->GetCurrentDirection() - pTankB->GetCurrentDirection() ) == 2 ){//
		return DWOOD_COLLISION_DIRECTION_OPPOSITE;
	}
	else if( ( abs( pTankA->GetCurrentDirection() - pTankB->GetCurrentDirection() ) % 2 ) == 1 ){
		return DWOOD_COLLISION_DIRECTION_CROSSED;
	}
	else{
		LOG_NOW("GetCollisionType Err");
	}

	return DWOOD_COLLISION_DIRECTION_NO;
}

void CWoodCollision::DoKiss(CWoodTank * pTankA, CWoodTank * pTankB){
	GLfloat fPreA = 0.0;
	GLfloat fPreB = 0.0;
	CWoodTank * pTankSpeedMin = GetMinSpeedTank(pTankA, pTankB);
	CWoodTank * pTankSpeedMax = GetMaxSpeedTank(pTankA, pTankB);
	if( pTankA->GetCurrentDirection() % 2 != 0 ){ //x direction
		fPreA = pTankA->GetPreX();
		fPreB = pTankB->GetPreX();
		//bIsXDiretion = true;
	}
	else{ // y direction
		fPreA = pTankA->GetPreY();
		fPreB = pTankB->GetPreY();
	}

	GLfloat fDistance = fabs(fPreA - fPreB);
	GLfloat fSpeedMaxRollBack = pTankSpeedMax->GetCurrentSpeed() - pTankSpeedMin->GetCurrentSpeed() - fDistance + pTankA->GetOneWidth() + pTankB->GetOneWidth();	
	pTankSpeedMax->RollBack(fSpeedMaxRollBack);
	pTankSpeedMax->GoOpposite();
}


void CWoodCollision::DoOpposite(CWoodTank * pTankA, CWoodTank * pTankB){
	GLfloat fPreA = 0.0;
	GLfloat fPreB = 0.0;
	GLfloat fTarget = 0.0;
	GLfloat fTureWillMoved = pTankA->GetCurrentSpeed() + pTankB->GetCurrentSpeed();	

	if( pTankA->GetCurrentSpeed() == 0.0 && pTankB->GetCurrentSpeed()==0.0 ){
		pTankA->GoOpposite();
		pTankB->GoOpposite();	
		return;
	}

	//bool bIsXDiretion = false;
	if( pTankA->GetCurrentDirection() % 2 != 0 ){ //x direction
		fPreA = pTankA->GetPreX();
		fPreB = pTankB->GetPreX();
		//bIsXDiretion = true;
	}
	else{ // y direction
		fPreA = pTankA->GetPreY();
		fPreB = pTankB->GetPreY();
	}

	GLfloat fDistance = fabs(fPreA - fPreB)-(pTankA->GetOneWidth() + pTankB->GetOneWidth());
	GLfloat fSpeedARollBack = pTankA->GetCurrentSpeed() - pTankA->GetCurrentSpeed()*fDistance/fTureWillMoved;
	GLfloat fSpeedBRollBack = fTureWillMoved - fDistance - fSpeedARollBack; 


	pTankA->RollBack( fSpeedARollBack+0.001 );
	pTankB->RollBack( fSpeedBRollBack+0.001 );

	pTankA->GoOpposite();
	pTankB->GoOpposite();


}

void CWoodCollision::DoCross(CWoodTank * pTankA, CWoodTank * pTankB){
	ST_WOOD_RECT stCollRect;
	memset(&stCollRect,0,sizeof(stCollRect));
	if( pTankA->GetCurrentDirection() % 2 == 0 ){  //A in Y axle	
		stCollRect.fXLeftDown = pTankA->GetX() - pTankA->GetOneWidth();
		stCollRect.fYLeftDown = pTankB->GetY() - pTankB->GetOneWidth();
		stCollRect.fXRightUp  = pTankA->GetX() + pTankA->GetOneWidth();
		stCollRect.fYRightUp  = pTankB->GetY() + pTankB->GetOneWidth();
	}
	else if( pTankB->GetCurrentDirection() % 2 == 0 ){  // B in Y axle
		stCollRect.fXLeftDown = pTankB->GetX() - pTankB->GetOneWidth();
		stCollRect.fYLeftDown = pTankA->GetY() - pTankA->GetOneWidth();
		stCollRect.fXRightUp  = pTankB->GetX() + pTankB->GetOneWidth();
		stCollRect.fYRightUp  = pTankA->GetY() + pTankA->GetOneWidth();
	}

	CWoodTank * pRollBackTank = NULL;
	if( CWoodTool::IsRectCrossed( pTankA->GetMyPreRect(), &stCollRect ) ){ //A is the first one; B need to roll back
		pRollBackTank = pTankB;
	}
	else{
		pRollBackTank = pTankA;
	}

	GLfloat fA = 0.0;
	GLfloat fB = 0.0;
	GLfloat fRollBackSpeed = 0.0;
	GLfloat fRectWidth = 0.0;
	if( pRollBackTank->GetCurrentDirection() % 2 != 0 ){ //x direction
		fA = pRollBackTank->GetPreX();
		fB = (stCollRect.fXLeftDown+stCollRect.fXRightUp) / 2;
		fRectWidth = fabs(stCollRect.fXRightUp-stCollRect.fXLeftDown)/2;

	}
	else{ // y direction
		fA = pRollBackTank->GetPreY();
		fB = (stCollRect.fYLeftDown+stCollRect.fYRightUp) / 2;		
		fRectWidth = fabs(stCollRect.fYRightUp-stCollRect.fYLeftDown)/2;
	}
	
	fRollBackSpeed = pRollBackTank->GetCurrentSpeed() - (fabs(fA - fB) - fRectWidth - pRollBackTank->GetOneWidth());

	pRollBackTank->RollBack( fRollBackSpeed );
}

void CWoodCollision::DoCollisionWithType(CWoodTank * pTankA, CWoodTank * pTankB,int iCollisionType){
	switch(iCollisionType){
		case DWOOD_COLLISION_DIRECTION_SAME:
			DoKiss(pTankA,pTankB);
			break;
		case DWOOD_COLLISION_DIRECTION_OPPOSITE:
			DoOpposite(pTankA,pTankB);
			break;
		case DWOOD_COLLISION_DIRECTION_CROSSED:
			DoCross(pTankA,pTankB);
			break;
		default:
			break;
	}
	pTankA->BeHit();
	pTankB->BeHit();
}

void CWoodCollision::HandelBullet( std::vector<wood::fire::tank::CWoodTank *> & vecAllTank,
					               std::vector<wood::fire::bullet::CWoodBullet *> & vecAllBullet ){


	//bullet kill tank...
	std::vector<CWoodTank *>::iterator itTank    = vecAllTank.begin();
	std::vector<CWoodTank *>::iterator itTankEnd = vecAllTank.end();	
	for(;itTank!=itTankEnd;++itTank){
		std::vector<CWoodBullet *>::iterator itBullet    = vecAllBullet.begin();
		std::vector<CWoodBullet *>::iterator itBulletEnd = vecAllBullet.end();

		for( ;itBullet!=itBulletEnd; ++itBullet ){
			if( CWoodTool::IsRectCrossed( (*itTank)->GetMyRect(), (*itBullet)->GetMyRect()) ){
				if( (*itTank)->GetId() != (*itBullet)->GetOwnerId() ){
					(*itTank)->BeFired();
					(*itBullet)->Boom();
				}				
			}
		}
	}		

	//Bullet kill bullet
	std::vector<CWoodBullet *>::iterator itBullet    = vecAllBullet.begin();
	std::vector<CWoodBullet *>::iterator itBulletEnd = vecAllBullet.end();
	for( ;itBullet!=itBulletEnd; ++itBullet ){
		std::vector<CWoodBullet *>::iterator itBulletInter    = itBullet;
		std::vector<CWoodBullet *>::iterator itBulletInterEnd = vecAllBullet.end();			
		for(;itBulletInter!=itBulletInterEnd; ++itBulletInter){
			if(itBulletInter == itBullet){				
				continue;
			}
			if( CWoodTool::IsRectCrossed( (*itBulletInter)->GetMyRect(), (*itBullet)->GetMyRect()) ){
				if( (*itBulletInter)->GetOwnerId() != (*itBullet)->GetOwnerId() ){
					(*itBullet)->Boom();
				}				
			}
		}
	}
}

void CWoodCollision::HandelCollision(std::vector<CWoodTank *> & vecWoodEnemy){
	std::vector<CWoodTank *>::iterator itEnemy    = vecWoodEnemy.begin();
	std::vector<CWoodTank *>::iterator itEnemyEnd = vecWoodEnemy.end();
	for(; itEnemy!=itEnemyEnd; ++itEnemy){
		std::vector<CWoodTank *>::iterator itEnemyInter    = itEnemy;
		std::vector<CWoodTank *>::iterator itEnemyInterEnd = vecWoodEnemy.end();
		for(;itEnemyInter!=itEnemyInterEnd; ++itEnemyInter){
			if(itEnemyInter == itEnemy){				
				continue;
			}

			if( CWoodTool::IsRectCrossed( (*itEnemy)->GetMyRect(), (*itEnemyInter)->GetMyRect()) ){					
				int iCollisionType = GetCollisionType( *itEnemy, *itEnemyInter);
				//LOG_NOW("CollisionType:"<<iCollisionType);
				DoCollisionWithType( *itEnemy, *itEnemyInter,iCollisionType);
			}	
		}
	}

}