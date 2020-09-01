#include <math.h>
#include <stdio.h>

#include <GL/glut.h>

float x, v, save, k = 0.05f;
bool is_spring = false;

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glTranslatef(x, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.5f, -0.5f);
    glVertex2f(0.5f, 0.5f);
    glVertex2f(-0.5f, 0.5f);
    glEnd();
    glLoadIdentity();
    glutSwapBuffers();
}

void timer(int value) {
    if (is_spring) {
        float a = -(x - save) * k;
        v += a;
        v *= 0.97f;
        x += v;
        if (v > -0.00001f && v < 0.00001f)
            is_spring = false;
    }
    glutPostRedisplay();
    glutTimerFunc(15, timer, 0);
}

void keyboard(GLubyte key, int x, int y) {
    save = 0.0f;
    v = 0.04f;
    x = save + v;
    is_spring = true;
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Catch Flies");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
}
