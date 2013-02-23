#include <iostream>
#include <iomanip>
#include <vector>
#include "CWoodTool.h"
#include "WoodFire_comm.h"
#include "CWoodTank.h"
#include "CWoodTankHelper.h"
#include "CWoodBullet.h"
#include "CWoodGlobal.h"
#include "CWoodFont.h"
#include "CWoodCollision.h"
#include "CWoodTimer.h"

using namespace std;

using namespace wood::fire::tank;
using namespace wood::tank::helper;
using namespace wood::fire::bullet;
using namespace wood::comm::tool;
using namespace wood::comm::font;
using namespace wood::comm::collision;
using namespace wood::comm::timer;



GLfloat g_fBulletSpeed = 0.02;

CWoodTank * g_pWoodTank       = NULL;
CWoodTankCom * g_pWoodTankCom = NULL;
CWoodBullet * g_pWoodBullet   = NULL;
std::vector<CWoodBullet *> g_vecWoodBullet;
std::vector<CWoodTank *>   g_vecAllTank;

CWoodCollision g_oWoodCollision;
CWoodTimer     g_oWoodTimer;
float          g_fPreTime = 0.0;
	

void WWDrawCordinator( void ){
	//start draw the cordinator
	glBegin(GL_LINES);
	{
		//draw x
		glLoadIdentity();
		glColor3f(1.0,0.0,0.0);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(2.0,0.0,0.0);

		//draw y
		glColor3f(0.0,1.0,0.0);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(0.0,2.0,0.0);

		//draw z
		glColor3f(0.0,0.0,1.0);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(0.0,0.0,2.0);
	}
	glEnd();
}




void WWInit(void) {
	CWOOD_FONT->Init();
	g_oWoodTimer.Init();

	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f ) ; //set the clear back ground color;
	glShadeModel(GL_SMOOTH); //GL_FLAT | GL_SMOOTH

	glEnable(GL_DEPTH_TEST);
	
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); //GL_FILL | GL_LINE

	glLineWidth(1.0);

	g_pWoodTank = new CWoodTank( 0.0, 0.0, 0.1, DWOOD_DIRECTION_UP,0);
	g_pWoodTank->SetTankType( DWOOD_TANK_TYPE_ENEMY_PLAYER_1 );

	g_vecAllTank.push_back(g_pWoodTank);

	//test xy collision	

/*
	g_pWoodTankCom = new CWoodTankCom( 0.5, 2.1, 0.00025,   DWOOD_DIRECTION_DOWN,3);
	g_vecAllTank.push_back(g_pWoodTankCom);
	
	g_pWoodTankCom = new CWoodTankCom( 0.5, 1.0, 0.00025*2, DWOOD_DIRECTION_UP,4);
	g_vecAllTank.push_back(g_pWoodTankCom);
*/	


	for(int i=0; i<13; i++){		
		g_pWoodTankCom = new CWoodTankCom( 1.1*(i+1), 0.5, 0.005*(i+1), DWOOD_DIRECTION_UP,i+1);
		g_vecAllTank.push_back(g_pWoodTankCom);
	}

	
	

	/*
	g_pWoodTankCom = new CWoodTankCom( 0.0, 0.0, 0.01, DWOOD_DIRECTION_UP);
	g_vecAllTank.push_back(g_pWoodTankCom);

	g_pWoodTankCom = new CWoodTankCom( 0.0, 0.0, 0.02, DWOOD_DIRECTION_UP);
	g_vecAllTank.push_back(g_pWoodTankCom);

	g_pWoodTankCom = new CWoodTankCom( 0.0, 0.0, 0.03, DWOOD_DIRECTION_UP);
	g_vecAllTank.push_back(g_pWoodTankCom);

	g_pWoodTankCom = new CWoodTankCom( 0.0, 0.0, 0.05, DWOOD_DIRECTION_UP);
	g_vecAllTank.push_back(g_pWoodTankCom);

	g_pWoodTankCom = new CWoodTankCom( 0.0, 0.0, 0.1, DWOOD_DIRECTION_UP);
	g_vecAllTank.push_back(g_pWoodTankCom);

	g_pWoodTankCom = new CWoodTankCom( 0.0, 0.0, 0.15, DWOOD_DIRECTION_UP);
	g_vecAllTank.push_back(g_pWoodTankCom);

	g_pWoodTankCom = new CWoodTankCom( 0.0, 0.0, 0.015, DWOOD_DIRECTION_UP);
	g_vecAllTank.push_back(g_pWoodTankCom);

	g_pWoodTankCom = new CWoodTankCom( 0.0, 0.0, 0.025, DWOOD_DIRECTION_UP);
	g_vecAllTank.push_back(g_pWoodTankCom);
	*/

}

void WWMotion(){	
	g_fBulletSpeed+=0.06;
	glutPostRedisplay();
}

void WWShow( void ) {
	g_fPreTime = g_oWoodTimer.GetTime();
	CWOOD_GLOBAL->m_fFlames ++ ;

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) ; //clear all pixels	

	glLoadIdentity();
	gluLookAt(1.0,-1.0,10.0, 0.0,0.0,0.0, 0.0,1.0,0.0);
	glPushMatrix();
	
	

	WWDrawCordinator();
	{
		g_pWoodTank->ShowNow();

		std::vector<CWoodTank *>::iterator itEnemy    = g_vecAllTank.begin();
		std::vector<CWoodTank *>::iterator itEnemyEnd = g_vecAllTank.end();
		for(; itEnemy!=itEnemyEnd; ++itEnemy){
			g_pWoodBullet = NULL;
			(*itEnemy)->AutoAction( g_pWoodBullet );
			if( g_pWoodBullet ){
				g_vecWoodBullet.push_back(g_pWoodBullet);
			}
		}

		g_oWoodCollision.HandelCollision(g_vecAllTank);


		std::vector<CWoodBullet *>::iterator itBullet    = g_vecWoodBullet.begin();
		for(; itBullet!=g_vecWoodBullet.end(); ){
			if( *itBullet ){
				if( (*itBullet)->IsKilled() || (*itBullet)->IsBoomed() ){  //need to erase itBullet...
					delete *itBullet;
					*itBullet = NULL;
					itBullet = g_vecWoodBullet.erase(itBullet);	
				}
				else{
					(*itBullet)->GoOn();
					itBullet ++ ;
				}
			}
		}

		g_oWoodCollision.HandelBullet(g_vecAllTank,g_vecWoodBullet );


		itEnemy    = g_vecAllTank.begin();
		itEnemyEnd = g_vecAllTank.end();
		for(; itEnemy!=itEnemyEnd; ++itEnemy){
			glPopMatrix();
			glPushMatrix();
			(*itEnemy)->ShowNow();
		}

		std::vector<CWoodBullet *>::iterator itBulletDone    = g_vecWoodBullet.begin();
		for(; itBulletDone!=g_vecWoodBullet.end(); ){
			if( *itBulletDone ){
				glPopMatrix();
				glPushMatrix();
				(*itBulletDone)->ShowNow();
				itBulletDone ++ ;
			}
		}



	}

	if( CWOOD_GLOBAL->m_fFlames == 10 ){		
		CWOOD_GLOBAL->m_fFPS = CWOOD_GLOBAL->m_fFlames/CWOOD_TIME->GetUsedTime();			
		CWOOD_GLOBAL->m_fFlames = 0;
	}

	glPopMatrix();
	glPushMatrix();
	CWOOD_FONT->ShowString( 10.0,14.0,"FPS:%.2f",CWOOD_GLOBAL->m_fFPS);
	CWOOD_FONT->ShowString( 10.0,10.0,"TIMER:%.2f",g_oWoodTimer.GetTime());

	glutSwapBuffers();

	while( g_oWoodTimer.GetTime() < g_fPreTime + 16.6f ){ //do one frame per X ms. 16.6 = 1000/60, 60FPS
	}	
}


void WWReShow( int iWidth, int iHeight ) {
	glViewport(0,0,(GLsizei)iWidth, (GLsizei)iHeight);
	glMatrixMode( GL_PROJECTION ); // GL_MODELVIEW |  GL_PROJECTION | GL_TEXTURE

	glLoadIdentity();
	glOrtho(FWOOD_BORDERLINE_LEFT,FWOOD_BORDERLINE_RIGHT,FWOOD_BORDERLINE_DOWN,FWOOD_BORDERLINE_UP,-100.0,100.0);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

void WWMouse(int iButton, int iState, int iX, int iY){
	if( iState == GLUT_DOWN ){
		switch( iButton ){
			case GLUT_LEFT_BUTTON:
				g_pWoodTank->GoLeft();
				break;
			case GLUT_RIGHT_BUTTON:
				g_pWoodTank->GoRight();
				break;
			case GLUT_MIDDLE_BUTTON:
				g_pWoodTank->Stop();				
				break;
			default:
				break;
		}
	}
}

void WWSpecialKeyboard(int iKey, int iMouseX, int iMouseY){
	//std::cout<<"KeyDown:"<<(int)iKey<<std::endl;
	switch(iKey) { 
		 case GLUT_KEY_UP :  
			 g_pWoodTank->GoUp();
			 break; 
		 case GLUT_KEY_DOWN:  
			 g_pWoodTank->GoDown();
			 break; 
		 case GLUT_KEY_LEFT:  
			 g_pWoodTank->GoLeft();
			 break; 
		 case GLUT_KEY_RIGHT:  
			 g_pWoodTank->GoRight();
			 break;
		 default:
			 g_pWoodTank->GoLeft();
			 break;
	} 
}

void WWKeyboard(unsigned char cKey,int iMouseX, int iMouseY){
	//std::cout<<"WWKeyboardDown:"<<(int)cKey<<std::endl;			
	switch(cKey) { 
		case 32: //space key.
			g_pWoodTank->Fire(g_pWoodBullet);
			g_vecWoodBullet.push_back(g_pWoodBullet);
			break;
		default:
			break;
	}
}

int main(int argc, char** argv) {

	//for test;
	//LOG_NOW(CWoodTool::IsRectCrossed(-.,0,4,4,2,2,6,6));

	glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE ) ;
	glutInitWindowSize(800,800);
	glutInitWindowPosition(100,100);
	glutCreateWindow( argv[0] ) ;

	WWInit();
	glutDisplayFunc( WWShow ) ;
	glutReshapeFunc( WWReShow );
	glutMouseFunc( WWMouse );
	glutKeyboardFunc( WWKeyboard );
	glutSpecialFunc( WWSpecialKeyboard );
	glutIdleFunc( WWMotion );
	glutMainLoop() ;
	return 0;
}
