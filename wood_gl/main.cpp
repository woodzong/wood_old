//#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <GL/glext.h>
#include "comm.h"


GLfloat g_fXSpeed = 0.0;
GLfloat g_fYSpeed = 0.0;
GLfloat g_fZSpeed = 0.0;

GLfloat g_fXSpeedOffset = 0.0;
GLfloat g_fYSpeedOffset = 0.0;
GLfloat g_fZSpeedOffset = 0.0;

int g_iRotateSpeed       = 0.0;
int g_iRotateSpeedOffset = 0.0;

GLuint g_iCacheList = 0;

void MakeCheckImage( void ){
	int i = 0;
	int j = 0;
	int c = 0;

	for (i = 0; i < G_IMG_TEX_HEIGHT; i++) {
		for (j = 0; j < G_IMG_TEX_WIDTH; j++) {
			c = (( ( (i&0x8) == 0 ) ^ (j&0x8) == 0 ) )*255;
			g_ImgTex[i][j][0] = (GLubyte) c;
			g_ImgTex[i][j][1] = (GLubyte) c;
			g_ImgTex[i][j][2] = (GLubyte) c;
			g_ImgTex[i][j][3] = (GLubyte) 255;
		}
	}
}


void DrawCordinator( void ){
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

void WoodInitLight( void ){

	glLightfv(GL_LIGHT0, GL_AMBIENT,  g_light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  g_light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, g_light_specular);

	glLightfv(GL_LIGHT1, GL_AMBIENT,  g_light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE,  g_light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, g_light1_specular);

	//glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.5);
	//glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.5);
	//glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.2);
	glLightf(GL_LIGHT1,  GL_SPOT_CUTOFF, 40.0);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, g_light1_spot_direction);
	glLightf(GL_LIGHT1,  GL_SPOT_EXPONENT,10.0);

	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT,g_ligth_lmodel_ambient);
	//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); //双面光照
	//glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

	glMaterialfv(GL_FRONT,GL_AMBIENT, g_no_mat);
	glMaterialfv(GL_FRONT,GL_DIFFUSE, g_mat_diffuse);
	glMaterialfv(GL_FRONT,GL_SPECULAR, g_mat_specular);
	glMaterialfv(GL_FRONT,GL_SHININESS, g_high_shininess);
	glMaterialfv(GL_FRONT,GL_EMISSION, g_no_mat);



	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
}

void WoodInit(void) {
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f ) ; //set the clear back ground color;
	glShadeModel(GL_SMOOTH); //GL_FLAT | GL_SMOOTH

	WoodInitLight();

	glEnable(GL_DEPTH_TEST);


	//start init for tex
	{
		//glEnable( GL_TEXTURE_2D );
		//glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE ); 
		MakeCheckImage();
		glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
		glGenTextures( 1, &g_uTexName );
		glBindTexture( GL_TEXTURE_2D, g_uTexName );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

		glTexImage2D( GL_TEXTURE_2D, 0 , GL_RGBA, G_IMG_TEX_WIDTH,G_IMG_TEX_HEIGHT,0, GL_RGBA, GL_UNSIGNED_BYTE,g_ImgTex  );
	}




	//glEnable(GL_MULTISAMPLE_ARB);
	//glEnable(GL_MULTISAMPLE);
	//glEnable(GL_POLYGON_SMOOTH);
	//glEnable(GL_LINE_SMOOTH);

	//glEnable ( GL_POLYGON_SMOOTH ); //GL_POLYGON_SMOOTH | GL_LINE_SMOOTH 
	//glEnable ( GL_LINE_SMOOTH );

	//glHint ( GL_LINE_SMOOTH, GL_DONT_CARE );
	//glHint ( GL_POLYGON_SMOOTH, GL_DONT_CARE );

	//glEnable ( GL_BLEND );

	//glBlendFunc ( GL_SRC_ALPHA_SATURATE, GL_ONE);


	/*
	glMatrixMode(GL_PROJECTION); //initialize viewing values...
	glLoadIdentity(); //??
	glOrtho(0.0,1.0,0.0,1.0,-1.0,1.0); //??
	*/
	glLineWidth(2.0);

	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	glColorPointer( 4,GL_FLOAT,0,g_colors );
	glVertexPointer( 3,GL_FLOAT,0,g_vertices );

	//glList Start
	{
		g_iCacheList = glGenLists( 1 );
		glNewList( g_iCacheList,GL_COMPILE );
		//glTexCoord2f(0.0,0.0);

		glEnable(GL_TEXTURE_2D);
		glBindTexture( GL_TEXTURE_2D, g_uTexName );

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

		glDisable(GL_TEXTURE_2D);
		/*
		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

		glBindTexture( GL_TEXTURE_2D, g_uTexName );
		GLUquadricObj * sphere = gluNewQuadric();
		gluQuadricOrientation(sphere, GLU_OUTSIDE);
		gluQuadricNormals(sphere,GLU_SMOOTH);
		gluQuadricTexture(sphere,GL_TRUE);
		gluSphere(sphere,1.0,50,50);
		//glutSolidSphere(1.0,50,50);
		*/

		

		glEndList();
	}




}

void WoodMotion(){
	g_fXSpeed+=g_fXSpeedOffset;
	g_fYSpeed+=g_fYSpeedOffset;
	g_fZSpeed+=g_fZSpeedOffset;
	g_iRotateSpeed += g_iRotateSpeedOffset;
	g_iRotateSpeed = g_iRotateSpeed % 360;
	glutPostRedisplay();
}

void WoodShow( void ) {
	//std::cout<<"in WoodShow now....!!"<<std::endl;
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) ; //clear all pixels

	//glEnable(GL_POLYGON_STIPPLE);
	//glPolygonStipple(g_fly);
	glLoadIdentity();
	gluLookAt(5.0,5.0,5.0, 0.0,0.0,0.0, 0.0,1.0,0.0);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); //GL_FILL | GL_LINE

	DrawCordinator();

	//glBegin(GL_POLYGON);
	{ //GL_LINE_LOOP, GL_LINES,GL_POLYGON		
		//glScalef(1.5,0.5,30.0);			


		glLightfv(GL_LIGHT1, GL_POSITION, g_light1_position);
		glTranslatef( g_fXSpeed,g_fYSpeed,g_fZSpeed );
		glRotatef( (GLfloat)g_iRotateSpeed, 0.0, 1.0, 0.0);		

		/*
		glDrawElements (GL_POLYGON, 4, GL_UNSIGNED_BYTE, g_indices[0]);
		glDrawElements (GL_POLYGON, 4, GL_UNSIGNED_BYTE, g_indices[1]);
		glDrawElements (GL_POLYGON, 4, GL_UNSIGNED_BYTE, g_indices[2]);
		glDrawElements (GL_POLYGON, 4, GL_UNSIGNED_BYTE, g_indices[3]);
		glDrawElements (GL_POLYGON, 4, GL_UNSIGNED_BYTE, g_indices[4]);
		glDrawElements (GL_POLYGON, 4, GL_UNSIGNED_BYTE, g_indices[5]);	
		//glLoadIdentity();
		glutSolidSphere(1.0,50,50);
		*/

		glCallList( g_iCacheList );

		//glLightfv(GL_LIGHT0, GL_POSITION, g_light_position);


		//glBlendFunc ( GL_SRC_ALPHA_SATURATE, GL_ONE);
		//glLoadIdentity();
		//glTranslatef(-2.0,0.0,0.0);
	}
	//glEnd();

	//glFlush() ;// flush the buffer , like endl in iostream...
	glutSwapBuffers();
}


void WoodReShow( int iWidth, int iHeight ) {
	//std::cout<<"in WoodReShow 2 now....!!"<<std::endl;

	glViewport(0,0,(GLsizei)iWidth, (GLsizei)iHeight);
	glMatrixMode(GL_PROJECTION); // GL_MODELVIEW |  GL_PROJECTION | GL_TEXTURE

	glLoadIdentity();
	glFrustum(-1.0,1.0,-1.0,1.0,2.5,3000.0);
	glMatrixMode(GL_MODELVIEW);

}

void WoodMouse(int iButton, int iState, int iX, int iY){
	if( iState == GLUT_DOWN ){
		switch( iButton ){
			case GLUT_LEFT_BUTTON:
				g_fXSpeedOffset -=0.001;
				break;
			case GLUT_RIGHT_BUTTON:
				g_fXSpeedOffset +=0.001;
				break;
			case GLUT_MIDDLE_BUTTON:
				g_fXSpeedOffset = 0.0;				
				break;
			default:
				break;
		}
	}
}

void WoodSpecialKeyboard(int iKey, int iMouseX, int iMouseY){
	std::cout<<"KeyDown:"<<(int)iKey<<std::endl;
	switch(iKey) { 
		 case GLUT_KEY_UP :  
			 g_fYSpeedOffset +=0.0001;
			 break; 
		 case GLUT_KEY_DOWN:  
			 g_fYSpeedOffset -=0.0001;
			 break; 
		 case GLUT_KEY_LEFT:  
			 g_fXSpeedOffset -=0.0001;
			 break; 
		 case GLUT_KEY_RIGHT:  
			 g_fXSpeedOffset +=0.0001;
			 break; 
		 case GLUT_KEY_F1:			 
			 g_iRotateSpeedOffset += 1;
			 break;
		 default:
			 g_iRotateSpeedOffset = 0;
			 g_fYSpeedOffset = 0;
			 g_fXSpeedOffset = 0;
			 break;
	} 
}

void WoodKeyboard(unsigned char cKey,int iMouseX, int iMouseY){

}

/*
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE ) ;
	glutInitWindowSize(800,800);
	glutInitWindowPosition(100,100);
	glutCreateWindow( argv[0] ) ;




	WoodInit();
	glutDisplayFunc( WoodShow ) ;
	glutReshapeFunc( WoodReShow );
	glutMouseFunc( WoodMouse );
	//glutKeyboardFunc( WoodKeyboard );
	glutSpecialFunc( WoodSpecialKeyboard );
	glutIdleFunc(WoodMotion);
	glutMainLoop() ;
	return 0;
}
*/
