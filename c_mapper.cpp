#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <GL/glut.h>

static int useFog = 0;
static int useQuads = 1;
static int tick = -1;

#define GREY	0
#define RED	    1
#define YELLOW	6
#define BLACK   7

//Boja materijala
static float materialColor[8][4] =
{
    {0.8, 0.8, 0.8, 1.0},
    {0.8, 0.0, 0.0, 1.0},
    {0.0, 0.8, 0.0, 1.0},
    {0.0, 0.0, 0.8, 1.0},
    {0.0, 0.8, 0.8, 1.0},
    {0.8, 0.0, 0.8, 1.0},
    {0.8, 0.8, 0.0, 1.0},
    {0.0, 0.0, 0.0, 0.6},
};

//Pozicija za svetlo
static float lightPos[4] = {2.0, 4.0, 2.0, 1.0};

//Podesavanje svetla
static float lightAmb[4] = {0.2, 0.2, 0.2, 1.0};
static float lightDiff[4] = {0.8, 0.8, 0.8, 1.0};
static float lightSpec[4] = {0.4, 0.4, 0.4, 1.0};

//Podesavanje pozicije dve povrsine
static float groundPlane[4] = {0.0, 1.0, 0.0, 1.499};
static float backPlane[4] = {0.0, 0.0, 1.0, 0.899};

//RGBA za fog
static float fogColor[4] = {0.0, 0.0, 0.0, 0.0};

//manual mode za kocku
int manual_mode = 0;

int move_horizontal = 0;
int move_vertical = 0;

int horizontal_flag = 0;
int vertical_flag = 0;

int rotate_flag = 0;

// Crna boja sa 50% intenziteta za sivkastu
static unsigned char shadowPattern[128] =
{
    0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55
};

//Podesavanje za tacke kocke(pozicije)
static float cube_vertexes[6][4][4] =
{
    {
        {-1.0, -1.0, -1.0, 1.0},
        {-1.0, -1.0, 1.0, 1.0},
        {-1.0, 1.0, 1.0, 1.0},
        {-1.0, 1.0, -1.0, 1.0}
    },

    {
        {1.0, 1.0, 1.0, 1.0},
        {1.0, -1.0, 1.0, 1.0},
        {1.0, -1.0, -1.0, 1.0},
        {1.0, 1.0, -1.0, 1.0}
    },

    {
        {-1.0, -1.0, -1.0, 1.0},
        {1.0, -1.0, -1.0, 1.0},
        {1.0, -1.0, 1.0, 1.0},
        {-1.0, -1.0, 1.0, 1.0}
    },

    {
        {1.0, 1.0, 1.0, 1.0},
        {1.0, 1.0, -1.0, 1.0},
        {-1.0, 1.0, -1.0, 1.0},
        {-1.0, 1.0, 1.0, 1.0}
    },

    {
        {-1.0, -1.0, -1.0, 1.0},
        {-1.0, 1.0, -1.0, 1.0},
        {1.0, 1.0, -1.0, 1.0},
        {1.0, -1.0, -1.0, 1.0}
    },

    {
        {1.0, 1.0, 1.0, 1.0},
        {-1.0, 1.0, 1.0, 1.0},
        {-1.0, -1.0, 1.0, 1.0},
        {1.0, -1.0, 1.0, 1.0}
    }
};

static float cube_normals[6][4] =
{
    {-1.0, 0.0, 0.0, 0.0},
    {1.0, 0.0, 0.0, 0.0},
    {0.0, -1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0, 0.0},
    {0.0, 0.0, -1.0, 0.0},
    {0.0, 0.0, 1.0, 0.0}
};

/**
* Fukcnika za boju
*/
static void setColor(int c)
{
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE, &materialColor[c][0]);
}

/**
* Funkcija za iscrtavanje kocke sa zadatom bojom
*
*/
static void drawCube(int color)
{
    int i;
    setColor(color);
    for (i = 0; i < 6; ++i)
    {
        glNormal3fv(&cube_normals[i][0]);
        glBegin(GL_POLYGON);
        glVertex4fv(&cube_vertexes[i][0][0]);
        glVertex4fv(&cube_vertexes[i][1][0]);
        glVertex4fv(&cube_vertexes[i][2][0]);
        glVertex4fv(&cube_vertexes[i][3][0]);
        glEnd();
    }
}

static void drawPlane(int w, int h, int color)
{
    static int initialized = 0;
    static int usedLighting = 0;
    static GLuint checklist = 0;

    if (!initialized)
    {
        static float square_normal[4] = {0.0, 0.0, 1.0, 0.0};
        static float square[4][4];
        int i, j;

        checklist = glGenLists(1);
        glNewList(checklist, GL_COMPILE_AND_EXECUTE);

        glNormal3fv(square_normal);
        glBegin(GL_QUADS);
        setColor(color);

        for (j = 0; j < h; ++j)
        {
            for (i = 0; i < w; ++i)
            {
                square[0][0] = -1.0 + 2.0 / w * i;
                square[0][1] = -1.0 + 2.0 / h * (j + 1);
                square[0][2] = 0.0;
                square[0][3] = 1.0;

                square[1][0] = -1.0 + 2.0 / w * i;
                square[1][1] = -1.0 + 2.0 / h * j;
                square[1][2] = 0.0;
                square[1][3] = 1.0;

                square[2][0] = -1.0 + 2.0 / w * (i + 1);
                square[2][1] = -1.0 + 2.0 / h * j;
                square[2][2] = 0.0;
                square[2][3] = 1.0;

                square[3][0] = -1.0 + 2.0 / w * (i + 1);
                square[3][1] = -1.0 + 2.0 / h * (j + 1);
                square[3][2] = 0.0;
                square[3][3] = 1.0;

                glVertex4fv(&square[0][0]);
                glVertex4fv(&square[1][0]);
                glVertex4fv(&square[2][0]);
                glVertex4fv(&square[3][0]);
            }
        }


        glEnd();
        glEndList();
        initialized = 1;
    }
    else
    {
        glCallList(checklist);
    }
}

/**
 * Funkcija za senku na povrsini
 */
static void calculateShadowPosition(float ground[4], float light[4])
{
    float dot;
    float shadowMat[4][4];

    dot = ground[0] * light[0] +
          ground[1] * light[1] +
          ground[2] * light[2] +
          ground[3] * light[3];

    shadowMat[0][0] = dot - light[0] * ground[0];
    shadowMat[1][0] = 0.0 - light[0] * ground[1];
    shadowMat[2][0] = 0.0 - light[0] * ground[2];
    shadowMat[3][0] = 0.0 - light[0] * ground[3];

    shadowMat[0][1] = 0.0 - light[1] * ground[0];
    shadowMat[1][1] = dot - light[1] * ground[1];
    shadowMat[2][1] = 0.0 - light[1] * ground[2];
    shadowMat[3][1] = 0.0 - light[1] * ground[3];

    shadowMat[0][2] = 0.0 - light[2] * ground[0];
    shadowMat[1][2] = 0.0 - light[2] * ground[1];
    shadowMat[2][2] = dot - light[2] * ground[2];
    shadowMat[3][2] = 0.0 - light[2] * ground[3];

    shadowMat[0][3] = 0.0 - light[3] * ground[0];
    shadowMat[1][3] = 0.0 - light[3] * ground[1];
    shadowMat[2][3] = 0.0 - light[3] * ground[2];
    shadowMat[3][3] = dot - light[3] * ground[3];

    glMultMatrixf((const GLfloat *) shadowMat);
}

void idle(void)
{
    tick++;
    if (tick >= 120)
    {
        tick = 0;
    }
    glutPostRedisplay();
}

/**
 * Funkcija za hendlovanje inputa
 */
void keyboard(unsigned char ch, int x, int y)
{
    switch (ch)
    {
    case 27:
        exit(0);
        break;
    case 'F':
    case 'f':
        useFog = !useFog;
        useFog ? glEnable(GL_FOG) : glDisable(GL_FOG);
        glutPostRedisplay();
        break;
    case ' ':
        rotate_flag = 1;
        idle();
        glutPostRedisplay();
        break;

    case 'a':
        horizontal_flag = 1;
        move_horizontal = -1;
        idle();
        glutPostRedisplay();
        break;

    case 'd':
        horizontal_flag = 1;
        move_horizontal = 1;
        idle();
        glutPostRedisplay();
        break;

    case 'w':
        vertical_flag = 1;
        move_vertical = 1;
        idle();
        glutPostRedisplay();
        break;

    case 's':
        vertical_flag = 1;
        move_vertical = -1;
        idle();
        glutPostRedisplay();
        break;

    case 'm':
        lightPos[0] = lightPos[0] + 0.3;
        idle();
        glutPostRedisplay();
        break;

    case 'n':
        lightPos[0] = lightPos[0] - 0.3;
        idle();
        glutPostRedisplay();
        break;

    case 'j':
        lightPos[1] = lightPos[1] + 0.3;
        idle();
        glutPostRedisplay();
        break;

    case 'k':
        lightPos[1] = lightPos[1] - 0.3;
        idle();
        glutPostRedisplay();
        break;
    }
}

void display(void)
{
    GLfloat cubeXform[4][4];
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Zemlja
    glPushMatrix();
    glTranslatef(0.0, -1.5, 0.0);
    glRotatef(-90.0, 1, 0, 0);
    glScalef(2.0, 2.0, 2.0);
    drawPlane(6, 6, YELLOW);
    glPopMatrix();

    // Pozadina
    glPushMatrix();
    glTranslatef(0.0, 0.0, -0.9);
    glScalef(2.0, 2.0, 2.0);
    drawPlane(6, 6, YELLOW);
    glPopMatrix();

    // Pozicija crvene kocke i crtanje
    glPushMatrix();
    glTranslatef(0.0, 0.2, 0.0);
    glScalef(0.3, 0.3, 0.3);
    if(manual_mode == 0)
    {
        glRotatef((360.0 / (60 * 1)) * tick, 1, 0, 0);
        glRotatef((360.0 / (60 * 2)) * tick, 0, 1, 0);
        glRotatef((360.0 / (60 * 4)) * tick, 0, 0, 1);
    }

    if(rotate_flag = 1){
        glRotatef((360.0 / (60 * 1)) * tick, 1, 0, 0);
        glRotatef((360.0 / (60 * 2)) * tick, 0, 1, 0);
        glRotatef((360.0 / (60 * 4)) * tick, 0, 0, 1);
        rotate_flag = 0;
    }

    if(horizontal_flag == 1)
    {
        glRotatef((360.0 / (60 * 2)) * tick * move_horizontal, 0, 1, 0);
        horizontal_flag = 0;
    }

    if(vertical_flag == 1)
    {
        glRotatef((360.0 / (60 * 1)) * tick * move_vertical, 1, 0, 0);
        vertical_flag = 0;
    }

    // Crtanje crvene kocke
    glScalef(1.0, 2.0, 1.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *) cubeXform);
    drawCube(RED);
    glPopMatrix();

    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);

    // Crtanje senke na podu
    glPushMatrix();
    calculateShadowPosition(groundPlane, lightPos);
    glTranslatef(0.0, 0.0, 2.0);
    glMultMatrixf((const GLfloat *) cubeXform);
    drawCube(BLACK);
    glPopMatrix();

    // Crtanje senke na zidu
    glPushMatrix();
    calculateShadowPosition(backPlane, lightPos);
    glTranslatef(0.0, 0.0, 2.0);
    glMultMatrixf((const GLfloat *) cubeXform);
    drawCube(BLACK);
    glPopMatrix();

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

    if (useFog)
        glEnable(GL_FOG);

    glutSwapBuffers();
}

/**
 * Funkcija za fog selection
 */
void fog_select(int fog)
{
    glFogf(GL_FOG_MODE, fog);
    glutPostRedisplay();
}

/**
 * Funkcija za meni na desnon
 */
void menu_select(int mode)
{
    switch (mode)
    {
    case 1:
        if(manual_mode == 1)
        {
            manual_mode = 0;
            glutIdleFunc(idle);
        }
        else
        {
            manual_mode = 1;
            glutIdleFunc(NULL);
        }
        glutPostRedisplay();
        break;
    case 2:
        useFog = !useFog;
        useFog ? glEnable(GL_FOG) : glDisable(GL_FOG);
        glutPostRedisplay();
        break;
    case 5:
        exit(0);
        break;
    }
}

int main(int argc, char **argv)
{
    int width = 650, height = 650;
    int i;
    int fog_menu;

    glutInitWindowSize(width, height);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("Projekat 2 - shadow mapping");

    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutIdleFunc(idle);

    fog_menu = glutCreateMenu(fog_select);
    glutAddMenuEntry("Linear fog", GL_LINEAR);
    glutAddMenuEntry("Exp fog", GL_EXP);
    glutAddMenuEntry("Exp^2 fog", GL_EXP2);

    glutCreateMenu(menu_select);
    glutAddMenuEntry("Manual mode", 1);
    glutAddMenuEntry("Toggle fog", 2);
    glutAddSubMenu("Fog type", fog_menu);
    glutAddMenuEntry("Quit", 5);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    /* setup context */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 3.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -2.0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiff);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpec);


    glEnable(GL_NORMALIZE);

    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_MODE, GL_EXP);
    glFogf(GL_FOG_DENSITY, 0.5);
    glFogf(GL_FOG_START, 1.0);
    glFogf(GL_FOG_END, 3.0);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glShadeModel(GL_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPolygonStipple((const GLubyte *) shadowPattern);
    glClearColor(0.0, 0.0, 0.0, 1);
    glClearIndex(0);
    glClearDepth(1);
    glutMainLoop();
    return 0;
}
