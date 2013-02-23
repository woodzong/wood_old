#include "CWoodBullet.h"

using namespace wood::fire::bullet;


CWoodBullet::CWoodBullet( GLfloat fX, GLfloat fY, GLfloat fSpeed, int iDirection, int iType, int iOwnerId):
m_fX(fX),m_fY(fY),m_fSpeed(fSpeed),m_iDirection(iDirection),m_iType(iType),m_iOwnerId(iOwnerId),m_iBoom(0),m_iBoomed(0)
{
	m_iUnitInCache = glGenLists( 1 );
	glNewList( m_iUnitInCache,GL_COMPILE );

	glutSolidSphere(0.15,3,3);

	glEndList();
}

CWoodBullet::~CWoodBullet(){
	glDeleteLists(m_iUnitInCache, 1);
}

ST_WOOD_RECT * CWoodBullet::GetMyRect(){
	m_stRect.fXLeftDown = m_fX-GetOneWidth();
	m_stRect.fYLeftDown = m_fY-GetOneWidth();
	m_stRect.fXRightUp  = m_fX+GetOneWidth();
	m_stRect.fYRightUp  = m_fY+GetOneWidth();
	return &m_stRect; 
}

void CWoodBullet::GoOn(){
	switch( m_iDirection ){
		case DWOOD_DIRECTION_UP:
			m_fY += m_fSpeed;
			break;
		case DWOOD_DIRECTION_DOWN:
			m_fY -= m_fSpeed;
			break;
		case DWOOD_DIRECTION_RIGHT:
			m_fX += m_fSpeed;
			break;
		case DWOOD_DIRECTION_LEFT:
			m_fX -= m_fSpeed;
			break;
	}
}

void CWoodBullet::Boom(){
	m_iBoomed ++;
	if( m_iBoom == 0 ){
		m_iBoom = 10;
	}
}

bool CWoodBullet::IsBoomed(){
	return (m_iBoomed>0 && m_iBoom==0);
}

void CWoodBullet::ShowNow(){	
	//LOG_NOW("Direction:"<<m_iDirection<<" X:"<<m_fX<<" Y:"<<m_fY);
	if( m_iBoom>0 ){
		glColor3f( 1.0,0.0,0.0 );
		m_fSpeed = 0.0;
		m_iBoom --;
	}
	else{
		glColor3f( 1.0,1.0,1.0 );
	}
	glTranslatef( m_fX, m_fY, 0 );
	glRotatef( m_iDirection * 90.0, 0.0, 0.0, 1.0);
	glCallList( m_iUnitInCache );
}

bool CWoodBullet::IsKilled(){
	if( m_fX < FWOOD_BORDERLINE_LEFT || m_fX > FWOOD_BORDERLINE_RIGHT){
		return true;
	}

	if( m_fY < FWOOD_BORDERLINE_DOWN || m_fY > FWOOD_BORDERLINE_UP){
		return true;
	}

	return false;
}

void CWoodBullet::SetSpeed( GLfloat fSpeed ){ 
	m_fSpeed = fSpeed; 
};

void CWoodBullet::SetDirection( int iDirection ){
	m_iDirection = iDirection;
};

void CWoodBullet::Stop(){
	m_iDirection = DWOOD_DIRECTION_NONE;
	//just do nothing temporarily
}