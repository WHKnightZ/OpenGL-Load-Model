
#include <stdlib.h>
#include <math.h>
#include "glut.h"
#include "ball.h"
#include "table.h"

Table table;
double cameraAngle1 = -90;
double cameraAngle2 = 25;

// handle key presses
void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
	case ' ':
		// reset game on Space key
		table.reset();
		break;
	case 27:
		// exit on Escape key
		exit(0);
		break;
	}
}

// handle special key presses
void special(int key, int x, int y)
{
	// rotate the camera
	switch(key)
	{
	case GLUT_KEY_LEFT:
		cameraAngle1 -= 5;
		break;
	case GLUT_KEY_RIGHT:
		cameraAngle1 += 5;
		break;
	case GLUT_KEY_UP:
		cameraAngle2 -= 5;
		break;
	case GLUT_KEY_DOWN:
		cameraAngle2 += 5;
	}

	// limit the angle
	if(cameraAngle2 < 10) cameraAngle2 = 10;
	if(cameraAngle2 > 80) cameraAngle2 = 80;
}

// handle mouse clicks
void mouse(int button, int state, int x, int y)
{
	// shoot by clicking
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if(!table.moving()) table.shoot();
	}
}

// handle mouse motion
void passive(int x, int y)
{
	// adjust stick angle by left/right mouse movement
	int centre = glutGet(GLUT_WINDOW_WIDTH) / 2;
	table.setStickAngle((x - centre) * 0.2 - cameraAngle1);
}

// draw scene
void display(void)
{
	// update the game to the current time
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	table.update(currentTime);

	// clear window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// window sizes
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);

	// set perspective projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double fieldOfView = 50.0;
	double aspectRatio = (double)width / (double)height;
	double nearPlane = 0.1;
	double farPlane = 100.0;
	gluPerspective(fieldOfView, aspectRatio, nearPlane, farPlane);

	// set camera
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(0, 0, -8);
	glRotated(cameraAngle2, 1, 0, 0);
	glRotated(cameraAngle1, 0, 1, 0);

	// set light colour and position
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat lightAmbient[4] = {0.3f, 0.3f, 0.3f, 1.0f};
	GLfloat lightDiffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat lightSpecular[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat lightPosition[4] = {0.0f, 4.0f, 0.0f, 1.0f};
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	// normalise normals automatically
	glEnable(GL_NORMALIZE);

	// cause material colours to track current colour
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	table.draw();

	// refresh window
	glutSwapBuffers();
	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	// initialise GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(1280, 720);
	glutCreateWindow("Billiard Ball game");

	// register callbacks
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutMouseFunc(mouse);
	glutPassiveMotionFunc(passive);
	glutDisplayFunc(display);

	quadricObject = gluNewQuadric();
	glutMainLoop();
	return 0;
}
