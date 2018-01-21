
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
#include "shaderLoader.h"

using namespace std;

#define PI 3.1415926
GLhandleARB	MyShader;
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
void LoadShaders();
int main(int argc, char** argv)
{
	string s = "as3";
	lights = new Lights(s + "\.light");
	view = new View(s + "\.view");

	glutInit(&argc, argv);
	glutInitWindowSize(view->viewport[2], view->viewport[3]);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
	//glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);

	glutCreateWindow("LAB4");
	glewInit();


	scene = new Scene(s + "\.scene");
	
	mouse = new Mouse();

	LoadShaders();

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


void LoadShaders()
{
	MyShader = glCreateProgram();
	if (MyShader != 0)
	{
		//ShaderLoad(MyShader, "../Examples/TextureMapping/TextureMapping.vert", GL_VERTEX_SHADER);
		//ShaderLoad(MyShader, "../Examples/TextureMapping/TextureMapping.frag", GL_FRAGMENT_SHADER);
		ShaderLoad(MyShader, "../MeshExample/TextureMapping.vert", GL_VERTEX_SHADER);
		ShaderLoad(MyShader, "../MeshExample/g.geom", GL_GEOMETRY_SHADER);
		ShaderLoad(MyShader, "../MeshExample/TextureMapping.frag", GL_FRAGMENT_SHADER);
	
	}
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
	
	
	
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(view->eye[0], view->eye[1], view->eye[2],
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

		glUseProgram(MyShader);

		GLint location = glGetUniformLocation(MyShader, "level");
		if (location == -1)
			printf("Cant find texture name: level\n");
		else
			glUniform1i(location,2);
		location = glGetUniformLocation(MyShader, "radius");
		if (location == -1)
			printf("Cant find texture name: radius\n");
		else
			glUniform1f(location, 1.0f);
		location = glGetUniformLocation(MyShader, "colorTexture");
		if (location == -1)
			printf("Cant find texture name: radius\n");
		else
			glUniform1i(location, 0);
		location = glGetUniformLocation(MyShader, "centroid1");
		if (location == -1)
			printf("Cant find texture name: centroid\n");
		else
			glUniform4f(location, 0,1,0,1);


		scene->set(MyShader);
		


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
		case'f':
			lights->ls[0].position[1]--;
			break;
		case'r':
			lights->ls[0].position[1]++;
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


