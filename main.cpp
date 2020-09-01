#include "main.h"

void Display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    // cubeObject.draw();

    cube.draw();

    glutSwapBuffers();
}

// int timer = 0;

void Timer(int value) {
    // timer++;
    // if (timer == 10) {
    //     timer = 0;
    //     cubes.push_back(Cube(-40.0f));
    // }
    // auto it = cubes.begin();
    // while (it != cubes.end()) {
    //     if (it->update())
    //         it++;
    //     else
    //         it = cubes.erase(it);
    // }
    cube.update();
    glutPostRedisplay();
    glutTimerFunc(INTERVAL, Timer, 0);
}

void special_down(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_DOWN:
        cube.move_down();
        break;
    case GLUT_KEY_UP:
        cube.move_up();
        break;
    }
}

void special_up(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_DOWN:
        cube.spring();
        break;
    case GLUT_KEY_UP:
        cube.spring();
        break;
    }
}

int main(int argc, char **argv) {
    srand(time(NULL));

    Init_Game();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glEnable(GL_MULTISAMPLE);
    glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
    glutSetOption(GLUT_MULTISAMPLE, 8);
    POS_X = (glutGet(GLUT_SCREEN_WIDTH) - WIDTH) >> 1;
    POS_Y = (glutGet(GLUT_SCREEN_HEIGHT) - HEIGHT) >> 1;
    glutInitWindowPosition(POS_X, POS_Y);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Catch Flies");
    Init_GL();
    glutDisplayFunc(Display);

    glutSpecialFunc(special_down);
    glutSpecialUpFunc(special_up);

    glutTimerFunc(0, Timer, 0);
    glutMainLoop();
    return 0;
}
