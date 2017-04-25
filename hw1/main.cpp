#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include "../GL/glew.h"
#include "../GL/freeglut.h"

#define WINDOW_WIDTH 512
#define WINDOW_HEIGHT 512

void init(void);
void display(void);
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void mouseButton(int button, int state, int x, int y);
void mouseMove(int x, int y);
void idle(void);

typedef struct _Ink {
	float r, g, b, a;
	float x, y;
	int size;
}Ink;

Ink ink[10000] = { 0 };
int inkNum = 0;
float colorR = 0.0f, colorG = 0.0f, colorB = 0.0f, colorA = 0.5f;
int colorMode = 0;
int inkSize = 12;
float interval = 0.0001;
float lastPosX, lastPosY;
bool mousePush = false;

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("NPR hw1 - 0556619");
	glutReshapeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);

	glewInit();

	init();

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);

	glutMainLoop();

	return 0;
}

void init(void) {
	// OpenGL, GLSL version
	printf("OpenGL version: %s\n", glGetString(GL_VERSION));
	printf("GLSL version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	printf("\n");

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glHint(GL_POINT_SMOOTH, GL_NICEST);
	glEnable(GL_POINT_SMOOTH);
	for (int i = 0; i < inkNum; i++) {
		glPointSize(ink[i].size);
		glColor4f(ink[i].r, ink[i].g, ink[i].b, ink[i].a);
		glBegin(GL_POINTS);
		glVertex3f(ink[i].x, ink[i].y, 0.0f);
		glEnd();
	}
	glutSwapBuffers();
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27:	//ESC
		exit(0);
		break;
	case 'a':	// ink darker
	case 'A':
		if (colorA < 1.0)
			colorA += 0.01;
		break;
	case 'z':	// ink lighter
	case 'Z':
		if (colorA > 0.0)
			colorA -= 0.01;
		break;
	case 's':	// ink size bigger
	case 'S':
		if (inkSize < 20)
			inkSize += 1;
		break;
	case 'x':	// ink size smaller
	case 'X':
		if (inkSize > 5)
			inkSize -= 1;
		break;
	case 'd':	// ink ineterval increase
	case 'D':
		if (interval < 0.001)
			interval += 0.0001;
		break;
	case 'c':	// ink interval decrease
	case 'C':
		if (interval > 0.0)
			interval -= 0.0001;
		break;
	case 'f':	// change colorMode
	case 'F':
		if (colorMode == 3)
			colorMode = 0;
		else
			colorMode++;

		if (colorMode == 0) {
			colorR = 0.0f;
			colorG = 0.0f;
			colorB = 0.0f;
		}
		else if (colorMode == 1) {
			colorR = 1.0f;
			colorG = 0.0f;
			colorB = 0.0f;
		}
		else if (colorMode == 2) {
			colorR = 0.0f;
			colorG = 1.0f;
			colorB = 0.0f;
		}
		else if (colorMode == 3) {
			colorR = 0.0f;
			colorG = 0.0f;
			colorB = 1.0f;
		}
		break;
	case 'v':
	case 'V':
		inkNum = 0;
		break;
	default:
		break;
	}
}

void mouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) {
		if (state != GLUT_UP) {
			lastPosX = 2.0 * x / glutGet(GLUT_WINDOW_WIDTH) - 1;
			lastPosY = -2.0 * y / glutGet(GLUT_WINDOW_HEIGHT) + 1;

			ink[inkNum].x = lastPosX;
			ink[inkNum].y = lastPosY;
			ink[inkNum].r = colorR;
			ink[inkNum].g = colorG;
			ink[inkNum].b = colorB;
			ink[inkNum].a = colorA;
			ink[inkNum].size = inkSize;
			inkNum++;

			mousePush = true;
		}
		else
			mousePush = false;
	}
}

void mouseMove(int x, int y)
{
	float posX = 2.0 * x / glutGet(GLUT_WINDOW_WIDTH) - 1,
		  posY = -2.0 * y / glutGet(GLUT_WINDOW_HEIGHT) + 1;

	if ((posX - lastPosX) * (posX - lastPosX) + (posY - lastPosY) * (posY - lastPosY) < interval)
		return;

	lastPosX = posX;
	lastPosY = posY;
	
	ink[inkNum].x = posX;
	ink[inkNum].y = posY;
	ink[inkNum].r = colorR;
	ink[inkNum].g = colorG;
	ink[inkNum].b= colorB;
	ink[inkNum].a = colorA;
	ink[inkNum].size = inkSize;
	inkNum++;
}

void idle(void)
{
	glutPostRedisplay();
}