
#include "Mesh.h"
#include "glut.h"
#include<vector>
#include<iostream>
#include "View.h"
#include "Scene.h"
#include "Lights.h"
#include "Mouse.h"
using namespace std;
vector<Mesh *>objects;
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
	string s = "scene2";
	scene = new Scene(s+"\.scene");
	for (int i = 0; i < scene->objNum; ++i)
	{
		objects.push_back(new Mesh(scene->objFileNames[i]));
	}
	lights = new Lights(s + "\.light");
	view = new View(s + "\.view");
	mouse = new Mouse();
	//object = new Mesh("box.obj");

	glutInit(&argc, argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("Mesh Example");
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

	// viewport transformation
	glViewport(0, 0, windowSize[0], windowSize[1]);

	// projection transformation
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(60.0, (GLfloat)windowSize[0]/(GLfloat)windowSize[1], 1.0, 1000.0);
	gluPerspective(view->fovy, (GLfloat)windowSize[0] / (GLfloat)windowSize[1], view->dNear, view->dFar);
	// viewing and modeling transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(	view->eye[0], view->eye[1],view->eye[2],
				view->vAt[0], view->vAt[1], view->vAt[2],
				view->vUp[0], view->vUp[1], view->vUp[2]
			 );    // up

	glRotated(view->Rangle, view->vUp[0], view->vUp[1], view->vUp[2]);

	//注意light位置的設定，要在gluLookAt之後
	light();

	glMatrixMode(GL_MODELVIEW);
	
	for (int k = 0; k < scene->objNum; ++k)
	{
		glPushMatrix();
		//glLoadIdentity();
		glTranslated(scene->mats[k].translation[0], scene->mats[k].translation[1], scene->mats[k].translation[2]);
		glRotated(scene->mats[k].angle, scene->mats[k].rotation[0], scene->mats[k].rotation[1], scene->mats[k].rotation[2]);
		glScaled(scene->mats[k].scale[0], scene->mats[k].scale[1], scene->mats[k].scale[2]);
		
		int lastMaterial = -1;
		for(size_t i=0;i < objects[k]->fTotal;++i)
		{
			
			// set material property if this face used different material
			if(lastMaterial != objects[k]->faceList[i].m)
			{
				lastMaterial = (int)objects[k]->faceList[i].m;
				glMaterialfv(GL_FRONT, GL_AMBIENT  , objects[k]->mList[lastMaterial].Ka);
				glMaterialfv(GL_FRONT, GL_DIFFUSE  , objects[k]->mList[lastMaterial].Kd);
				glMaterialfv(GL_FRONT, GL_SPECULAR , objects[k]->mList[lastMaterial].Ks);
				glMaterialfv(GL_FRONT, GL_SHININESS, &objects[k]->mList[lastMaterial].Ns);

				//you can obtain the texture name by object->mList[lastMaterial].map_Kd
				//load them once in the main function before mainloop
				//bind them in display function here
			}

			glBegin(GL_TRIANGLES);
			for (size_t j=0;j<3;++j)
			{
				//textex corrd. object->tList[object->faceList[i][j].t].ptr
				glNormal3fv(objects[k]->nList[objects[k]->faceList[i][j].n].ptr);
				glVertex3fv(objects[k]->vList[objects[k]->faceList[i][j].v].ptr);
			}
			glEnd();


		}
		glPopMatrix();
	}

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