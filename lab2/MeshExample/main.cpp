
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
	string s = "Chess";
	lights = new Lights(s + "\.light");
	view = new View(s + "\.view");

	glutInit(&argc, argv);
	glutInitWindowSize(view->viewport[2], view->viewport[3]);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
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

void display()
{
	// clear the buffer
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);      //清除用color
	glClearDepth(1.0f);                        // Depth Buffer (就是z buffer) Setup
	glEnable(GL_DEPTH_TEST);                   // Enables Depth Testing
	glDepthFunc(GL_LEQUAL);                    // The Type Of Depth Test To Do
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//這行把畫面清成黑色並且清除z buffer

	// viewing and modeling transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(	view->eye[0], view->eye[1],view->eye[2],
				view->vAt[0], view->vAt[1], view->vAt[2],
				view->vUp[0], view->vUp[1], view->vUp[2]
			 );    // up

	glRotated(view->Rangle, view->vUp[0], view->vUp[1], view->vUp[2]);
	// viewport transformation
	// projection transformation
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(60.0, (GLfloat)windowSize[0]/(GLfloat)windowSize[1], 1.0, 1000.0);
	gluPerspective(view->fovy, (GLfloat)view->viewport[2] / (GLfloat)view->viewport[3], view->dNear, view->dFar);

	glViewport(view->viewport[0], view->viewport[1], view->viewport[2], view->viewport[3]);


	//注意light位置的設定，要在gluLookAt之後
	light();

	scene->set();

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