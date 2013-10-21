
// The OpenGL libraries, make sure to include the GLUT and OpenGL frameworks
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <math.h>
#include <string.h>
#include "glm/glm.hpp"
#include "glm/gtx/random.hpp"
#include "glm/gtc/type_precision.hpp"

using glm::vec2;
using glm::vec3;
using glm::i32vec2;

int lasttime;
int thistime;

float wheel_ang = 0.0f;

vec2 mousepos;
int buttons = 0;

float vertsCoords[] = {0.5f, 0.5f, 0.5f,          //V0
                                      -0.5f, 0.5f, 0.5f,           //V1
                                      -0.5f, -0.5f, 0.5f,         //V2
                                       0.5f, -0.5f, 0.5f,         //V3
                                       0.5f, -0.5f, -0.5f,       //V4
                                       0.5f,  0.5f, -0.5f,       //V5
                                      -0.5f, 0.5f, -0.5f,       //V6
                                      -0.5f, -0.5f, -0.5f,     //V7
                        };  
GLubyte indices[] = {0, 1, 2, 3,              //Front face
                                5, 0, 3, 4,             //Right face
                                5, 6, 7, 4,             //Back face
                                5, 6, 1, 0,             //Upper face
                                1, 6, 7, 2,              //Left face
                                7, 4, 3, 2,             //Bottom face
                      };                     
            
void display()
{
	thistime = glutGet(GLUT_ELAPSED_TIME);
	if ((thistime - lasttime) < 10)
		return;

	lasttime = thistime;
	int millis = thistime;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	static const char scroller[] = 
"                                                                                                                                                                                                                                                                                                                                                                       "\
"Hackers/LHB Cracking to your mind.        We removed the protections,     and we removed your mental blocks       we lowered your inhibitions and modified your state of being          with our trainers specifically designed to tap into the innards of your skulls, you can achieve things you have never believed to have been possible before....     "\
"                                                                                                                                                                                                                                                                                                                                                                       ";
	glBegin(GL_QUADS);
	// aseta tekstuuri

	int charoffs = millis / 50;
	int sx, sy;
	for (sy = 0; sy < 25; sy++)
	{
		for (sx = 0; sx < 12; sx++)
		{
			int cha = scroller[sy * 12 + sx + charoffs];
			if (cha == 0)
				goto LOPPUXD;
			int cx = cha & 15;
			int cy = cha / 16;
			float tx = (float)cx / 16.f;
			float ty = (float)cy / 16.f;
			float d = 1.0f / 16.f;
			float xd = (1.0f / 40.f) * 2.0f;
			float yd = -(1.0f / 25.f) * 2.0f;
			float x1 = ((float)sx / 40.f) * 2.0f - 1.0f;
			float y1 = 2.0f - ((float)sy / 25.f) * 2.0f - 1.0f;
			glColor3f(tx, ty, 0.f);
			glVertex2f(x1, y1);
			glColor3f(tx+d, ty, 0.f);
			glVertex2f(x1+xd, y1);
			glColor3f(tx+d, ty+d, 0.f);
			glVertex2f(x1+xd, y1+yd);
			glColor3f(tx, ty+d, 0.f);
			glVertex2f(x1, y1+yd);
		}
	}
	LOPPUXD:;
	glEnd();



	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float base = 2.f + sin(millis * 0.0076f) * 0.4f + sin(millis * 0.0023f) * 0.2f;
	glFrustum(-1.0f, 1.0f, 1.0f, -1.0f, 0.0f + base, 1.f + base);
	glScalef(1.0f, 640.f/480.f, 1.0f);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0.f, 0.f, -2.f);
	glRotatef(millis * 0.08f * 1.f, 0.4f, 0.4f, 0.4f);
	glRotatef(sin(millis * 0.001f) * 100.f, 0.7f, 0.1f, 0.2f);



	int i;
	for (i = 0; i < 6; i++)
	{
		int pb = i * 4;
		float x1 = vertsCoords[indices[pb + 0] * 3 + 0];
		float xd = vertsCoords[indices[pb + 2] * 3 + 0] - vertsCoords[indices[pb + 0] * 3 + 0];
		float y1 = vertsCoords[indices[pb + 0] * 3 + 1];
		float yd = vertsCoords[indices[pb + 2] * 3 + 1] - vertsCoords[indices[pb + 0] * 3 + 1];
		float z1 = vertsCoords[indices[pb + 0] * 3 + 1];
		float zd = vertsCoords[indices[pb + 2] * 3 + 1] - vertsCoords[indices[pb + 0] * 3 + 2];

		glLineWidth(10.0f + sin(millis * 0.00331f + i) * 8.0f);

	glBegin(GL_LINE_LOOP);
	glColor3f(0.1f, 0.3f, 0.5f);
		glVertex3f(x1,y1,z1);
		glVertex3f(x1+xd,y1+yd,z1+zd);
	glColor3f(0.7f, 0.2f, 0.2f);
		glVertex3f(x1+xd,y1,z1);
		glVertex3f(x1,y1+yd,z1+zd);
	glColor3f(0.1f, 0.6f, 0.9f);
		glVertex3f(x1,y1,z1+zd);
		glVertex3f(x1+xd,y1+yd,z1);
	glColor3f(1.0f, 0.4f, 0.2f);
	glEnd();
	}

	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();



	glColor3f(0.5f, 0.2f, 0.0f);
	glBegin(GL_LINES);
	glColor3f(0.1f, 0.6f, 0.9f);
	glVertex2f(-0.4f, -1.0f);
	glColor3f(1.0f, 0.4f, 0.2f);
	glVertex2f(-0.4f, 1.0f);
	glEnd();

	glutSwapBuffers();
}

// Called every time a window is resized to resize the projection matrix
void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
//    glFrustum(-0.1, 0.1, -(float)h/(10.0*(float)w), (float)h/(10.0f*(float)w), 0.2f, 9999999.0f);
}

void keyboard(unsigned char key, int x, int y)
{
	if (key == 'q')
		exit(0);
}

void init() // Called before main loop to set up the program
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glDisable(GL_DEPTH_TEST);
}

void mouse_pos(int x, int y)
{
	mousepos.x = (float)x / 800.f;
	mousepos.y = (float)y / 800.f;
}

void mouse(int button, int state, int x, int y)
{
	int bit;
	switch(button)
	{
	case GLUT_LEFT_BUTTON:
		bit = 1;
		break;
	case GLUT_RIGHT_BUTTON:
		bit = 2;
		break;
	case GLUT_MIDDLE_BUTTON:
		bit = 4;
		break;
	}

	if (state == GLUT_UP)
		buttons &= ~bit;
	else if (state == GLUT_DOWN)
		buttons |= bit;

	mousepos.x = (float)x / 800.f;
	mousepos.y = (float)y / 800.f;
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv); // initializes glut

	// Sets up a double buffer with RGBA components and a depth component
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);

	// Sets the window size to 512*512 square pixels
	glutInitWindowSize(640, 480);

	// Sets the window position to the upper left
	glutInitWindowPosition(0, 0);

	// Creates a window using internal glut functionality
	glutCreateWindow("Hello!");

	// passes reshape and display functions to the OpenGL machine for callback
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutKeyboardFunc(keyboard);

	glutMotionFunc(mouse_pos);
	glutMouseFunc(mouse);
	init();

	// Starts the program.
	glutMainLoop();
	return 0;
}
