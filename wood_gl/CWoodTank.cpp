#include <time.h>
#include <stdlib.h>
#include "CWoodTank.h"
#include "CWoodFont.h"

using namespace wood::fire::tank;


CWoodTank::CWoodTank( GLfloat fX, GLfloat fY, GLfloat fSpeed, int iDirection, int iId):
m_fX(fX),m_fY(fY),m_fSpeed(fSpeed),m_iDirection(iDirection),m_iId(iId),m_iTurnCount(0),m_fMovedDistance(0),m_fFiredRef(0),
m_fPreX(fX),m_fPreY(fY),m_iPreDirection(iDirection),m_fCurrentSpeed(0),m_iIsHit(0),m_iIsFired(0)
{
	m_fColor[0] = 0.0f;
	m_fColor[1] = 1.0f;
	m_fColor[2] = 0.0f;

	CWOOD_FONT->Init();

	GLfloat g_vertices[] = {
		-0.5,-0.5,0.5,
		0.5,-0.5,0.5,
		0.5,0.5,0.5,
		-0.5,0.5,0.5,
		0.5,-0.5,-0.5,
		-0.5,-0.5,-0.5,
		0.5,0.5,-0.5,
		-0.5,0.5,-0.5
	};


	GLfloat g_colors[] = {
		1.0, 1.0, 1.0, 1.0,
		1.0, 1.0, 1.0, 1.0,
		1.0, 1.0, 1.0, 1.0,
		1.0, 1.0, 1.0, 1.0,
		1.0, 1.0, 1.0, 1.0,
		1.0, 1.0, 1.0, 1.0,
		1.0, 1.0, 1.0, 1.0,
		1.0, 1.0, 1.0, 1.0,
	};

	GLubyte g_indices[][4] = {
		{ 0,1,2,3 }, //前
		{ 0,1,4,5 }, //下
		{ 1,4,6,2 }, //右
		{ 3,2,6,7 }, //上
		{ 0,5,7,3 }, //左
		{ 5,4,6,7 }, //后
	};

	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	glColorPointer( 4,GL_FLOAT,0,g_colors );
	glVertexPointer( 3,GL_FLOAT,0,g_vertices );

	m_iUnitInCache = glGenLists( 1 );	
	
	glNewList( m_iUnitInCache,GL_COMPILE );

	glBegin(GL_TRIANGLES);
	glVertex3f(0.0,0.5,0.0);
	glVertex3f(-0.5,0.0,0.0);
	glVertex3f(0.5,0.0,0.0);
	glEnd();
	
	glLineWidth(1.0);
	glNormal3f( 0.0f, 0.0f, 1.0f);
	glDrawElements (GL_POLYGON, 4, GL_UNSIGNED_BYTE, g_indices[0]); //前

	glNormal3f( 0.0f, -1.0f, 0.0f);
	glDrawElements (GL_POLYGON, 4, GL_UNSIGNED_BYTE, g_indices[1]); //下

	glNormal3f( 1.0f, 0.0f, 0.0f);
	glDrawElements (GL_POLYGON, 4, GL_UNSIGNED_BYTE, g_indices[2]); //右

	glNormal3f( 0.0f, 1.0f, 0.0f);
	glDrawElements (GL_POLYGON, 4, GL_UNSIGNED_BYTE, g_indices[3]); //上

	glNormal3f( -1.0f, 0.0f, 0.0f);
	glDrawElements (GL_POLYGON, 4, GL_UNSIGNED_BYTE, g_indices[4]); //左

	glNormal3f( 0.0f, 0.0f, -1.0f);
	glDrawElements (GL_POLYGON, 4, GL_UNSIGNED_BYTE, g_indices[5]);	//后		

	glEndList();
}


CWoodTank::~CWoodTank(){
	glDeleteLists(m_iUnitInCache, 1);
}

void CWoodTank::ShowNow(){
	CWOOD_FONT->ShowString(m_fX,m_fY,"%d",m_iId);
	glTranslatef( m_fX, m_fY, 0 );
	glRotatef( m_iDirection * 90.0, 0.0, 0.0, 1.0);

	if( m_iIsFired>0 ){
		glLineWidth(10.0);
		m_fColor[0] = 1.0f;
		m_fColor[1] = 0.0f;
		m_fColor[2] = 0.0f;
		m_iIsFired --;
	}	
	else if(m_iIsHit>0){
		glLineWidth(4.0);
		m_fColor[0] = 1.0f;
		m_fColor[1] = 1.0f;
		m_fColor[2] = 0.0f;
		m_iIsHit --;
	}	
	else{
		glLineWidth(1.0);
		m_fColor[0] = 0.0f;
		m_fColor[1] = 1.0f;
		m_fColor[2] = 0.0f;
	}
	

	glColor3f( m_fColor[0], m_fColor[1], m_fColor[2] );
	glCallList( m_iUnitInCache );
	m_fPreX = m_fX;
	m_fPreY = m_fY;
	m_iPreDirection    = m_iDirection;
}

ST_WOOD_RECT * CWoodTank::GetMyRect(){
	m_stRect.fXLeftDown = m_fX-GetOneWidth();
	m_stRect.fYLeftDown = m_fY-GetOneWidth();
	m_stRect.fXRightUp  = m_fX+GetOneWidth();
	m_stRect.fYRightUp  = m_fY+GetOneWidth();
	return &m_stRect; 
}

ST_WOOD_RECT * CWoodTank::GetMyPreRect(){
	m_stPreRect.fXLeftDown = m_fPreX-GetOneWidth();
	m_stPreRect.fYLeftDown = m_fPreY-GetOneWidth();
	m_stPreRect.fXRightUp  = m_fPreX+GetOneWidth();
	m_stPreRect.fYRightUp  = m_fPreY+GetOneWidth();
	return &m_stPreRect; 
}

int CWoodTank::GetCurrentDirection(){
	return m_iDirection;
}

void CWoodTank::SetSpeed( GLfloat fSpeed ){ 
	m_fSpeed = fSpeed;
	m_fCurrentSpeed = m_fSpeed;
};

void CWoodTank::SetTankType( int iTankType ){
	m_iTankType = iTankType;
}

GLfloat CWoodTank::GetCurrentSpeed(){
	return m_fCurrentSpeed;
}

void CWoodTank::SetDirection( int iDirection ){
	m_iDirection = iDirection;
};

void CWoodTank::GoOpposite(){ 	//LOG_NOW("GoOpposite:"<<m_iDirection);
	//m_fSpeed = 0;
	switch(m_iDirection){
		case DWOOD_DIRECTION_UP:
			GoDown();
			break;
		case DWOOD_DIRECTION_DOWN:
			GoUp();			
			break;
		case DWOOD_DIRECTION_LEFT:
			GoRight();
			break;
		case DWOOD_DIRECTION_RIGHT:
			GoLeft();
			break;
	}
}

void CWoodTank::BeHit(){
	m_iIsHit = 20;
}

void CWoodTank::BeFired(){
	m_iIsFired = 10;
}

void CWoodTank::RollBack( GLfloat fDistance ){
	switch( m_iDirection ){
		case DWOOD_DIRECTION_UP:
			m_fY -= fDistance;
			break;
		case DWOOD_DIRECTION_DOWN:
			m_fY += fDistance;
			break;
		case DWOOD_DIRECTION_LEFT:
			m_fX += fDistance;
			break;
		case DWOOD_DIRECTION_RIGHT:
			m_fX -= fDistance;
			break;		
	}
}

void CWoodTank::GoUp(){
	if( m_iDirection != DWOOD_DIRECTION_UP ){
		m_fCurrentSpeed = 0.0;
		SetDirection(DWOOD_DIRECTION_UP);
	}
	else{
		m_fCurrentSpeed = m_fSpeed;
	}	
	m_fY += m_fCurrentSpeed ;
}

void CWoodTank::GoDown(){
	if( m_iDirection != DWOOD_DIRECTION_DOWN ){
		m_fCurrentSpeed = 0.0;
		SetDirection(DWOOD_DIRECTION_DOWN);
	}
	else{
		m_fCurrentSpeed = m_fSpeed;
	}

	m_fY -= m_fCurrentSpeed;
}

void CWoodTank::GoRight(){
	if( m_iDirection != DWOOD_DIRECTION_RIGHT ){
		m_fCurrentSpeed = 0.0;
		SetDirection(DWOOD_DIRECTION_RIGHT);
	}
	else{
		m_fCurrentSpeed = m_fSpeed;
	}	

	m_fX += m_fCurrentSpeed;
}

void CWoodTank::GoLeft(){
	if( m_iDirection != DWOOD_DIRECTION_LEFT ){
		m_fCurrentSpeed = 0.0;
		SetDirection(DWOOD_DIRECTION_LEFT);
	}
	else{
		m_fCurrentSpeed = m_fSpeed;
	}	

	m_fX -= m_fCurrentSpeed;
}

void CWoodTank::Stop(){
	//just do nothing temporarily
}

bool CWoodTank::IsReachedBorderline(){
	if( m_fX < FWOOD_BORDERLINE_LEFT+GetOneWidth() || m_fX > FWOOD_BORDERLINE_RIGHT-GetOneWidth()){
		return true;
	}

	if( m_fY < FWOOD_BORDERLINE_DOWN+GetOneWidth() || m_fY > FWOOD_BORDERLINE_UP-GetOneWidth()){
		return true;
	}

	return false;
}

void CWoodTank::FixtoBorderLine(){
	if( m_fX < FWOOD_BORDERLINE_LEFT+GetOneWidth() ){
		m_fX = FWOOD_BORDERLINE_LEFT+GetOneWidth();
	}

	if( m_fX > FWOOD_BORDERLINE_RIGHT-GetOneWidth() ){
		m_fX = FWOOD_BORDERLINE_RIGHT-GetOneWidth();
	}

	if( m_fY < FWOOD_BORDERLINE_DOWN+GetOneWidth() ){
		m_fY = FWOOD_BORDERLINE_DOWN+GetOneWidth();
	}

	if( m_fY > FWOOD_BORDERLINE_UP-GetOneWidth() ){
		m_fY = FWOOD_BORDERLINE_UP-GetOneWidth();
	}
}

void CWoodTank::FixToXCollistion( GLfloat fX){
	if( m_iDirection == DWOOD_DIRECTION_RIGHT ){
		m_fX = fX - GetOneWidth();
	}
	else if( m_iDirection == DWOOD_DIRECTION_LEFT ){
		m_fX = fX + GetOneWidth();
	}
}

void CWoodTank::FixToYCollistion( GLfloat fY){
	if( m_iDirection == DWOOD_DIRECTION_UP ){
		m_fY = fY - GetOneWidth();
	}
	else if( m_iDirection == DWOOD_DIRECTION_DOWN ){
		m_fY = fY + GetOneWidth();
	}
}

void CWoodTank::GoStraight(){
	m_fMovedDistance += m_fCurrentSpeed;
	m_fFiredRef      += m_fCurrentSpeed;
	switch( m_iDirection ){
		case DWOOD_DIRECTION_UP:
			GoUp();
			break;
		case DWOOD_DIRECTION_DOWN:
			GoDown();
			break;
		case DWOOD_DIRECTION_RIGHT:
			GoRight();
			break;
		case DWOOD_DIRECTION_LEFT:
			GoLeft();
			break;
	}

}

wood::fire::bullet::CWoodBullet * CWoodTank::Fire(  wood::fire::bullet::CWoodBullet * &pWoodBullet ){
	//LOG_NOW("Fire And New CWoodBullet");
	pWoodBullet = new wood::fire::bullet::CWoodBullet(m_fX,m_fY,0.05,m_iDirection,m_iTankType,m_iId);
	return pWoodBullet;
}

void CWoodTank::HVPreCloseTo(CWoodTank * pTankTarget ){
	GLfloat fXt = GetPreX() - pTankTarget->GetX();
	GLfloat fYt = GetPreY() - pTankTarget->GetY();
	GLfloat fOffset = (pTankTarget->GetOneWidth()+GetOneWidth());

	if( fXt > fOffset ){ //at the right side of target tank.
		LOG_NOW("right");
		m_fX = pTankTarget->GetX() + fOffset;
	}
	else if( fXt < -1*fOffset ){  // left side.
		LOG_NOW("left");
		m_fX = pTankTarget->GetX() - fOffset;
	}
	else if( fYt > fOffset ){ // up side.
		LOG_NOW("up");
		m_fY = pTankTarget->GetY() + fOffset;
	}
	else if( fYt < -1*fOffset ){ // down side.
		LOG_NOW("down");
		m_fY = pTankTarget->GetY() - fOffset;
	}

}




void CWoodTankCom::AutoAction( wood::fire::bullet::CWoodBullet * &pWoodBullet ){
	if( IsReachedBorderline() ){
		FixtoBorderLine();
		srand( GetRandomNum() ); //生成种子
		SetDirection( rand()%4 );
		m_iTurnCount ++;
	}
	else if( m_fMovedDistance > 18 ){  //add random trun logic...
		//LOG_NOW("m_fMovedDistance:"<<m_fMovedDistance);
		srand( GetRandomNum() ); //生成种子
		SetDirection( rand()%4 );
		m_fMovedDistance = 0;
		m_iTurnCount ++;
	}

	if( m_fFiredRef > 3 ){
		m_fFiredRef = 0;
		srand( GetRandomNum() ); //生成种子
		if( rand()%3 == 1 ){
			pWoodBullet = new wood::fire::bullet::CWoodBullet(m_fX,m_fY,m_fCurrentSpeed + 0.02,m_iDirection,m_iTankType,m_iId);
		}
	}

	GoStraight();
}

