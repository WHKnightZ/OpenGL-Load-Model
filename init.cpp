#include "main.h"

void Init_Game() {}

const GLfloat Light_Pos[] = {25.0f, 15.0f, 20.0f, 0.0f}; // -27, 25, 29
// const GLfloat Light_Pos2[] = {21.0f, -19.0f, -24.0f, 0.0f}; // 27, -25, -29
const GLfloat Light_Dif[] = {1.0f, 1.0f, 1.0f, 1.0f};

void Init_GL() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, Light_Dif);
    glLightfv(GL_LIGHT0, GL_POSITION, Light_Pos);
    // glEnable(GL_LIGHT1);
    // glLightfv(GL_LIGHT1, GL_DIFFUSE, Light_Dif);
    // glLightfv(GL_LIGHT1, GL_POSITION, Light_Pos2);
    // glEnable(GL_COLOR_MATERIAL);
    glMaterialf(GL_FRONT, GL_SHININESS, 90.0f);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.0, 1.0, 1.0, 100.0);
    glTranslatef(0.0f, 0.0f, -10.0f);
    glRotatef(20.0f, 1.0f, -1.0f, 0.0f);
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_COLOR_MATERIAL);

    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glEnable(GL_LINE_SMOOTH);
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // glutIgnoreKeyRepeat(GL_TRUE);
    // glEnable(GL_TEXTURE_2D);

    glEnable(GL_DEPTH_TEST);
}
