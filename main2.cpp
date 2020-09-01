#include <math.h>
#include <time.h>

#include <GL/freeglut.h>
#include <GL/glext.h>

#include <vector>

#include "../Library/loadpng.h"
#include "../Library/process_image.h"

#include "../Library/gl_texture.h"

#define WIDTH 600
#define HEIGHT 600

#define INTERVAL 15

int POS_X, POS_Y;

Image texture;

struct Face {
    int vertices[4];
    int texcoords[4];
    int normal;

    Face(int vertex0, int vertex1, int vertex2, int vertex3, int texcoord0, int texcoord1, int texcoord2, int texcoord3,
         int normal) {
        vertices[0] = vertex0;
        vertices[1] = vertex1;
        vertices[2] = vertex2;
        vertices[3] = vertex3;
        texcoords[0] = texcoord0;
        texcoords[1] = texcoord1;
        texcoords[2] = texcoord2;
        texcoords[3] = texcoord3;
        this->normal = normal;
    }
};

std::vector<float *> vertices;
std::vector<float *> texcoords;
std::vector<float *> normals;
std::vector<Face> faces;

float angle = 0.0f;

GLuint list = 1;
GLuint texture1, texture2;

void Display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    // glBindTexture(GL_TEXTURE_2D, 0);
    glTranslatef(-10.0f, 0.0f, 0.0f);
    glRotatef(angle, 0.0f, 1.0f, 0.0f);
    // srand(time(0));
    for (int i = 0; i < 800; i++) {
        // glTranslatef((rand() % 100) / 100.0f - 0.5f, (rand() % 100) / 100.0f - 0.5f, 0.0f);
        for (auto face : faces) {
            glBindTexture(GL_TEXTURE_2D, texture1);
            // Map_Texture(&texture);
            glBegin(GL_QUADS);
            glNormal3fv(normals[face.normal]);
            glTexCoord2fv(texcoords[face.texcoords[0]]);
            glVertex3fv(vertices[face.vertices[0]]);
            glTexCoord2fv(texcoords[face.texcoords[1]]);
            glVertex3fv(vertices[face.vertices[1]]);
            glTexCoord2fv(texcoords[face.texcoords[2]]);
            glVertex3fv(vertices[face.vertices[2]]);
            glTexCoord2fv(texcoords[face.texcoords[3]]);
            glVertex3fv(vertices[face.vertices[3]]);
            glEnd();
        }
        // glutSolidCube(1.0);
    }
    // glCallList(list);
    glutSwapBuffers();
}

bool render = false;
int fps = 0;
int timer = 0, timer2 = 0;
int start = clock(), end;

void Idle() {
    end = clock();
    timer += end - start;
    start = end;
    if (timer >= 15) {
        angle += 2.0f * timer / 15;
        timer2 += timer;
        if (timer2 >= 1000) {
            timer2 = 0;
            printf("%d\n", fps);
            fps = 0;
        }
        timer = 0;
        fps++;
        glutPostRedisplay();
    }
}

// void Timer(int value) {
//     timer += INTERVAL;
//     angle += 0.5f;
//     if (timer >= 1000) {
//         timer = 0;
//         printf("%d\n", fps);
//         fps = 0;
//     }
//     render = true;
//     glutTimerFunc(INTERVAL, Timer, 0);
// }

const GLfloat Light_Pos[] = {-21.0f, 19.0f, 24.0f, 1.0f};   // -27, 25, 29
const GLfloat Light_Pos2[] = {21.0f, -19.0f, -24.0f, 1.0f}; // 27, -25, -29
const GLfloat Light_Dif[] = {1.0f, 1.0f, 1.0f, 1.0f};

void Init_GL() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    // glLightfv(GL_LIGHT0, GL_DIFFUSE, Light_Dif);
    glLightfv(GL_LIGHT0, GL_POSITION, Light_Pos);
    // glEnable(GL_LIGHT1);
    // glLightfv(GL_LIGHT1, GL_DIFFUSE, Light_Dif);
    // glLightfv(GL_LIGHT1, GL_POSITION, Light_Pos2);
    // glEnable(GL_COLOR_MATERIAL);
    // glMaterialf(GL_FRONT, GL_SHININESS, 90.0f);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, 1.0, -1.0);
    // gluPerspective(20.0, 1.0, 1.0, 100.0);

    // glOrtho(-1.0, 1.0, -1.0, 1.0, 1.0, 1000.0);
    // glTranslatef(0.0f, 0.0f, -50.0f);
    glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glutIgnoreKeyRepeat(GL_TRUE);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);

    Load_Texture(&texture, "texture.png");
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.w, texture.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.img);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    Load_Texture(&texture, "texture2.png");
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.w, texture.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.img);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    // Map_Texture(&texture);

    // vertices.push_back(new float[3]{-0.5f, -0.5f, 0.0f});
    // vertices.push_back(new float[3]{0.5f, -0.5f, 0.0f});
    // vertices.push_back(new float[3]{0.5f, 0.5f, 0.0f});
    // vertices.push_back(new float[3]{-0.5f, 0.5f, 0.0f});
    // vertices.push_back(new float[3]{0.0f, 0.0f, 0.0f});

    // texcoords.push_back(new float[2]{0.0f, 0.0f});
    // texcoords.push_back(new float[2]{1.0f, 0.0f});
    // texcoords.push_back(new float[2]{1.0f, 1.0f});
    // texcoords.push_back(new float[2]{0.0f, 1.0f});
    // texcoords.push_back(new float[2]{0.5f, 0.5f});

    // normals.push_back(new float[3]{0.0f, 0.0f, 1.0f});
    // faces.push_back(Face(0, 3, 4, 0, 3, 4, 0));
    // faces.push_back(Face(1, 2, 4, 1, 2, 4, 0));

    vertices.push_back(new float[3]{-0.5f, -0.5f, -0.5f}); // 0
    vertices.push_back(new float[3]{0.5f, -0.5f, -0.5f});  // 1
    vertices.push_back(new float[3]{0.5f, 0.5f, -0.5f});   // 2
    vertices.push_back(new float[3]{-0.5f, 0.5f, -0.5f});  // 3
    vertices.push_back(new float[3]{-0.5f, -0.5f, 0.5f});  // 4
    vertices.push_back(new float[3]{0.5f, -0.5f, 0.5f});   // 5
    vertices.push_back(new float[3]{0.5f, 0.5f, 0.5f});    // 6
    vertices.push_back(new float[3]{-0.5f, 0.5f, 0.5f});   // 7

    texcoords.push_back(new float[2]{0.0f, 0.0f});
    texcoords.push_back(new float[2]{1.0f, 0.0f});
    texcoords.push_back(new float[2]{1.0f, 1.0f});
    texcoords.push_back(new float[2]{0.0f, 1.0f});

    normals.push_back(new float[3]{0.0f, 0.0f, -1.0f});
    normals.push_back(new float[3]{0.0f, 0.0f, 1.0f});
    normals.push_back(new float[3]{-1.0f, 0.0f, 0.0f});
    normals.push_back(new float[3]{1.0f, 0.0f, 0.0f});
    normals.push_back(new float[3]{0.0f, -1.0f, 0.0f});
    normals.push_back(new float[3]{0.0f, 1.0f, 0.0f});

    faces.push_back(Face(0, 1, 2, 3, 0, 1, 2, 3, 0));
    faces.push_back(Face(4, 5, 6, 7, 0, 1, 2, 3, 1));
    faces.push_back(Face(0, 3, 7, 4, 0, 1, 2, 3, 2));
    faces.push_back(Face(1, 2, 6, 5, 0, 1, 2, 3, 3));
    faces.push_back(Face(0, 1, 5, 4, 0, 1, 2, 3, 4));
    faces.push_back(Face(2, 3, 7, 6, 0, 1, 2, 3, 5));

    // list = glGenLists(1); // hoặc list = 1 bởi vì glGenLists => 1 vào lần đầu gọi, 2 vào lần thứ
    // 2 ... nhưng thực ra sẽ khác nếu có list bị xóa, if (glIsList(1)) => nếu list = 1 tồn tại trả
    // về true

    // glNewList(list, GL_COMPILE);
    // for (int i = 0; i < 2000; i++) {
    //     // glTranslatef((rand() % 100) / 100.0f - 0.5f, (rand() % 100) / 100.0f - 0.5f, 0.0f);
    //     // for (auto face : faces) {
    //     //     // glBindTexture(GL_TEXTURE_2D, rand() % 2 ? texture1 : texture2);
    //     //     // Map_Texture(&texture);
    //     //     glBegin(GL_QUADS);
    //     //     glNormal3fv(normals[face.normal]);
    //     //     glTexCoord2fv(texcoords[face.texcoords[0]]);
    //     //     glVertex3fv(vertices[face.vertices[0]]);
    //     //     glTexCoord2fv(texcoords[face.texcoords[1]]);
    //     //     glVertex3fv(vertices[face.vertices[1]]);
    //     //     glTexCoord2fv(texcoords[face.texcoords[2]]);
    //     //     glVertex3fv(vertices[face.vertices[2]]);
    //     //     glTexCoord2fv(texcoords[face.texcoords[3]]);
    //     //     glVertex3fv(vertices[face.vertices[3]]);
    //     //     glEnd();
    //     // }
    //     glutSolidCube(1.0);
    // }
    // glEndList();
}

int main(int argc, char **argv) {
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

    glutIdleFunc(Idle);
    Init_GL();
    glutDisplayFunc(Display);
    // glutTimerFunc(0, Timer, 0);
    glutMainLoop();
    return 0;
}
