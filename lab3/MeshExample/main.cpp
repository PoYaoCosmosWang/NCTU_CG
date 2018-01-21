
#include "Mesh.h"
#include "glew.h"
#include "glut.h"
#include "freeimage.h"
#include<vector>
#include<iostream>
#include "View.h"
#include "Scene.h"
#include "Lights.h"
#include "Mouse.h"

using namespace std;

#define PI 3.1415926

View* view;
Scene* scene;
Lights* lights;
Mouse* mouse;
int windowSize[2];

void light();
void display();
void reshape(GLsizei , GLsizei );

void keyBoardEvent(unsigned char, int, int);
void motionEvt(int, int);
void pasMotionEvt(int, int);
int main(int argc, char** argv)
{
	string s = "CornellBox";
	lights = new Lights(s + "\.light");
	view = new View(s + "\.view");

	glutInit(&argc, argv);
	glutInitWindowSize(view->viewport[2], view->viewport[3]);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
	//glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);

	glutCreateWindow("LAB2");
	glewInit();


	scene = new Scene(s + "\.scene");
	
	mouse = new Mouse();


	glutKeyboardFunc(keyBoardEvent);
	glutMotionFunc(motionEvt);
	glutPassiveMotionFunc(pasMotionEvt);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();

	return 0;
}

void light()
{
	/*GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat light_ambient[] = {0.0, 0.0, 0.0, 1.0};
	GLfloat light_position[] = {150.0, 150.0, 150.0, 1.0};
	*/
	glShadeModel(GL_SMOOTH);

	// z buffer enable
	glEnable(GL_DEPTH_TEST);

	// enable lighting
	glEnable(GL_LIGHTING);
	// set light property
	int i = 0;
	for (i = 0; i < lights->lightNum; ++i)
	{
		glEnable(GL_LIGHT0+i);
		glLightfv(GL_LIGHT0+i, GL_POSITION, lights->ls[i].position);
		glLightfv(GL_LIGHT0+i, GL_DIFFUSE, lights->ls[i].diffuse);
		glLightfv(GL_LIGHT0+i, GL_SPECULAR, lights->ls[i].specular);
		glLightfv(GL_LIGHT0+i, GL_AMBIENT, lights->ls[i].ambient);
	}
	glEnable(GL_LIGHT0 + i);
	glLightfv(GL_LIGHT0 + i, GL_AMBIENT, lights->ambient);

}



void drawScene()
{
	glMatrixMode(GL_MODELVIEW);
	for (int i = 0; i < 10; ++i)
	{
		scene->objs[i].setObj();
	}
}

void drawMirror()
{
	glMatrixMode(GL_MODELVIEW);
	scene->objs[10].setObj();
	scene->objs[11].setObj();
}

void display()
{
	// clear the buffer
	glClearAccum(0, 0, 0, 0);
	glClearDepth(1.0f);                        // Depth Buffer (就是z buffer) Setup
	glEnable(GL_DEPTH_TEST);                   // Enables Depth Testing

	glEnable(GL_STENCIL_TEST); //Enable using the stencil buffer
	int initial_value = 0;
	glClearStencil(initial_value);

	glDepthFunc(GL_LEQUAL);                    // The Type Of Depth Test To Do
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);      //清除用color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT|| GL_STENCIL_BUFFER_BIT | GL_ACCUM_BUFFER_BIT);//這行把畫面清成黑色並且清除z buffer

	Vec3 dir(view->vAt[0] - view->eye[0], view->vAt[1] - view->eye[1], view->vAt[2] - view->eye[2]);
	Vec3 right(-dir[2], 0, dir[0]);
	right.normalize();
	int N = 8;
	float dis= 0.2;
	for (int i = 0; i < N; ++i)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
		
		Vec3 jitter = right*cos(PI*2.0/N*i)*dis+view->vUp*sin(PI*2.0/N*i)*dis;
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(view->eye[0]+jitter[0], view->eye[1]+jitter[1], view->eye[2]+jitter[2],
			view->vAt[0], view->vAt[1], view->vAt[2],
			view->vUp[0], view->vUp[1], view->vUp[2]
		);    
		glTranslatef(view->vAt[0], view->vAt[1], view->vAt[2]);
		glRotated(view->Rangle, view->vUp[0], view->vUp[1], view->vUp[2]);
		glTranslatef(-view->vAt[0], -view->vAt[1], -view->vAt[2]);
		// viewport transformation
		// projection transformation
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(view->fovy, (GLfloat)view->viewport[2] / (GLfloat)view->viewport[3], view->dNear, view->dFar);
		glViewport(view->viewport[0], view->viewport[1], view->viewport[2], view->viewport[3]);
		light();

		
		glColorMask(0, 0, 0, 0); //Disable drawing colors to the screen
		glStencilFunc(GL_EQUAL, 0, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
		drawMirror();
		glClear(GL_DEPTH_BUFFER_BIT);

		//2nd B mirror
		//glColorMask(1.0, 1.0, 1.0, 1.0);
		glStencilFunc(GL_EQUAL, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
		glPushMatrix();
		glTranslatef(80, 0, 0);
		glScalef(-1, 1, 1);
		drawMirror();
		glPopMatrix();
		glClear(GL_DEPTH_BUFFER_BIT);

		//3nd B mirror
		//glColorMask(1.0, 1.0, 1.0, 1.0);
		glStencilFunc(GL_EQUAL, 2, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
		glPushMatrix();
		glTranslatef(160, 0, 0);
		glScalef(1, 1, 1);
		drawMirror();
		glPopMatrix();
		glClear(GL_DEPTH_BUFFER_BIT);

		//4nd  B mirror
		//glColorMask(1.0, 1.0, 1.0, 1.0);
		glStencilFunc(GL_EQUAL, 3, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
		glPushMatrix();
		glTranslatef(240, 0, 0);
		glScalef(-1, 1, 1);
		drawMirror();
		glPopMatrix();
		glClear(GL_DEPTH_BUFFER_BIT);
		

		//2nd mirror
		//glColorMask(1.0, 1.0, 1.0, 1.0);
		glStencilFunc(GL_EQUAL, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
		glPushMatrix();
		glTranslatef(-80, 0, 0);
		glScalef(-1, 1, 1);
		drawMirror();
		glPopMatrix();
		glClear(GL_DEPTH_BUFFER_BIT);

		//3nd mirror
		//glColorMask(1.0, 1.0, 1.0, 1.0);
		glStencilFunc(GL_EQUAL, 2, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
		glPushMatrix();
		glTranslatef(-160, 0, 0);
		glScalef(1, 1, 1);
		drawMirror();
		glPopMatrix();
		glClear(GL_DEPTH_BUFFER_BIT);

		//4nd mirror
		//glColorMask(1.0, 1.0, 1.0, 1.0);
		glStencilFunc(GL_EQUAL, 3, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
		glPushMatrix();
		glTranslatef(-240, 0, 0);
		glScalef(-1, 1, 1);
		drawMirror();
		glPopMatrix();
		glClear(GL_DEPTH_BUFFER_BIT);


		glColorMask(1.0, 1.0, 1.0, 1.0);

		//4nd reflection
		glStencilFunc(GL_LEQUAL, 4, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glPushMatrix();
		glTranslatef(-320, 0, 0);
		glScalef(1, 1, 1);
		drawScene();
		glPopMatrix();

		//3nd reflection
		glStencilFunc(GL_LEQUAL, 3, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glPushMatrix();
		glTranslatef(-240, 0, 0);
		glScalef(-1, 1, 1);
		drawScene();
		glPopMatrix();

		//2nd reflection
		glStencilFunc(GL_LEQUAL, 2, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glPushMatrix();
		glTranslatef(-160, 0, 0);
		glScalef(1, 1, 1);
		drawScene();
		glPopMatrix();
		

		glColorMask(1.0, 1.0, 1.0, 1.0);
		//1st reflection
		glStencilFunc(GL_LEQUAL, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glPushMatrix();
		glTranslatef(-80, 0, 0);
		glScalef(-1, 1, 1);
		drawScene();
		glPopMatrix();
		//glClear(GL_DEPTH_BUFFER_BIT);

		//4nd  B reflection
		glStencilFunc(GL_LEQUAL, 4, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glPushMatrix();
		glTranslatef(320, 0, 0);
		glScalef(1, 1, 1);
		drawScene();
		glPopMatrix();

		//3nd B reflection
		glStencilFunc(GL_LEQUAL, 3, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glPushMatrix();
		glTranslatef(240, 0, 0);
		glScalef(-1, 1, 1);
		drawScene();
		glPopMatrix();

		//2nd B reflection
		glStencilFunc(GL_LEQUAL, 2, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glPushMatrix();
		glTranslatef(160, 0, 0);
		glScalef(1, 1, 1);
		drawScene();
		glPopMatrix();


		glColorMask(1.0, 1.0, 1.0, 1.0);
		//1st B reflection
		glStencilFunc(GL_LEQUAL, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glPushMatrix();
		glTranslatef(80, 0, 0);
		glScalef(-1, 1, 1);
		drawScene();
		glPopMatrix();
		//glClear(GL_DEPTH_BUFFER_BIT);


		//reality
		glColorMask(1.0, 1.0, 1.0, 1.0); //Disable drawing colors to the screen
		glStencilFunc( GL_LEQUAL,0, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		drawScene();
		//glClear(GL_DEPTH_BUFFER_BIT);

		


		glAccum(i ? GL_ACCUM : GL_LOAD, 1.0 / N);
		
	}
	/*
	// viewing and modeling transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(	view->eye[0], view->eye[1],view->eye[2],
				view->vAt[0], view->vAt[1], view->vAt[2],
				view->vUp[0], view->vUp[1], view->vUp[2]
			 );    // up

	glTranslatef(view->vAt[0], view->vAt[1], view->vAt[2]);
	glRotated(view->Rangle, view->vUp[0], view->vUp[1], view->vUp[2]);
	glTranslatef(-view->vAt[0], -view->vAt[1], -view->vAt[2]);
	
	// viewport transformation
	// projection transformation
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(60.0, (GLfloat)windowSize[0]/(GLfloat)windowSize[1], 1.0, 1000.0);
	gluPerspective(view->fovy, (GLfloat)view->viewport[2] / (GLfloat)view->viewport[3], view->dNear, view->dFar);

	glViewport(view->viewport[0], view->viewport[1], view->viewport[2], view->viewport[3]);
	

	//注意light位置的設定，要在gluLookAt之後
	light();
	//glTranslatef(40, 20,0);
	scene->set();
	*/

	
	glAccum(GL_RETURN, 1);
	glutSwapBuffers();
}

void reshape(GLsizei w, GLsizei h)
{
	windowSize[0] = w;
	windowSize[1] = h;
}

void keyBoardEvent(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'w':
			view->zoom(-1);
			break;
		case's':
			view->zoom(1);
			break;
		case'a':
			view->rotate(1);
			break;
		case'd':
			view->rotate(-1);
			break;
		case'1':
			view->vAt[0] = -10;
			view->vAt[1] = 12;
			view->vAt[2] = 0;
			break;
		case'2':
			view->vAt[0] = -50;
			view->vAt[1] = 12;
			view->vAt[2] = 0;
			break;
		case'3':
			view->vAt[0] = -400;
			view->vAt[1] = 12;
			view->vAt[2] = 0;
			break;
	}
	if ('1' <= key&&key <= '9')
	{

		scene->select(key - '1');
	}
	glutPostRedisplay();
}

void motionEvt(int x, int y)
{
	mouse->motionEvent(x, y, scene);
	glutPostRedisplay();
}


void pasMotionEvt(int x, int y)
{
	mouse->passiceMotionEvent(x, y);
}


