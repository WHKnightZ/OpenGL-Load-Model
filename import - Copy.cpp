#include <math.h>
#include <time.h>

#include <GL/freeglut.h>

#include <GL/glext.h>

#include <fstream>
#include <map>
#include <vector>

#include "../Library/loadpng.h"
#include "../Library/process_image.h"

#include "../Library/gl_texture.h"

#define WIDTH 600
#define HEIGHT 600

#define INTERVAL 15

#define MODEL_NAME "Models/Lowpoly_tree_sample.obj"

int POS_X, POS_Y;

class Model {
  private:
    static int count_char(std::string &str, char ch) {
        int c = 0;
        int length = str.length() - 1;
        for (int i = 0; i < length; i++) {
            if (str[i] == ch)
                c++;
        }
        return c;
    }

    static bool has_double_slash(std::string &str) {
        int length = str.length() - 2;
        for (int i = 0; i < length; i++) {
            if (str[i] == '/' && str[i + 1] == '/')
                return true;
        }
        return false;
    }

    class Material {
      public:
        float *ambient;
        float *diffuse;
        float *specular;

        Material(float *ambient, float *diffuse, float *specular) {
            this->ambient = ambient;
            this->diffuse = diffuse;
            this->specular = specular;
        }
    };

    class Face {
      public:
        int edge;
        int *vertices;
        int *texcoords;
        int normal;

        Face(int edge, int *vertices, int *texcoords, int normal = -1) {
            this->edge = edge;
            this->vertices = vertices;
            this->texcoords = texcoords;
            this->normal = normal;
        }
    };

    std::vector<Material> materials;
    std::map<std::string, int> map_material;

    std::vector<float *> vertices;
    std::vector<float *> texcoords;
    std::vector<float *> normals;
    std::vector<Face> faces;

    bool has_texcoord, has_normal;
    GLuint texture;
    GLuint list;

    void (Model::*add_face_3)(std::string &line), (Model::*add_face_4)(std::string &line);

    void load_material(const char *filename) {
        std::string line;
        std::vector<std::string> lines;
        std::ifstream in(filename);
        if (!in.is_open()) {
            printf("Cannot load material %s\n", filename);
            return;
        }

        while (!in.eof()) {
            std::getline(in, line);
            lines.push_back(line);
        }
        in.close();

        int count_material = 0;
        char str[40];
        std::string material;
        float *a, *d, *s;
        for (std::string &line : lines) {
            if (line[0] == 'n' && line[1] == 'e') {
                sscanf(line.c_str(), "newmtl %s", str);
                material = str;
                map_material[material] = count_material;
                count_material++;
                a = new float[4]{0.2f, 0.2f, 0.2f, 1.0f};
                d = new float[4]{0.8f, 0.8f, 0.8f, 1.0f};
                s = new float[4]{0.0f, 0.0f, 0.0f, 1.0f};
                materials.push_back(Material(a, d, s));
            } else if (line[0] == 'K') {
                switch (line[1]) {
                case 'a':
                    sscanf(line.c_str(), "Ka %f %f %f", &a[0], &a[1], &a[2]);
                    break;
                case 'd':
                    sscanf(line.c_str(), "Kd %f %f %f", &d[0], &d[1], &d[2]);
                    break;
                case 's':
                    sscanf(line.c_str(), "Ks %f %f %f", &s[0], &s[1], &s[2]);
                    break;
                }
            }
        }
    }

    void add_face_3v(std::string &line) {
        int v0, v1, v2;
        sscanf(line.c_str(), "f %d %d %d", &v0, &v1, &v2);
        int *v = new int[3]{v0 - 1, v1 - 1, v2 - 1};
        faces.push_back(Face(3, v, NULL));
    }

    void add_face_3vt(std::string &line) {
        int v0, v1, v2, t0, t1, t2;
        sscanf(line.c_str(), "f %d/%d %d/%d %d/%d", &v0, &t0, &v1, &t1, &v2, &t2);
        int *v = new int[3]{v0 - 1, v1 - 1, v2 - 1};
        int *t = new int[3]{t0 - 1, t1 - 1, t2 - 1};
        faces.push_back(Face(3, v, t));
    }

    void add_face_3vn(std::string &line) {
        int v0, v1, v2, n;
        sscanf(line.c_str(), "f %d//%d %d//%d %d//%d", &v0, &n, &v1, &n, &v2, &n);
        int *v = new int[3]{v0 - 1, v1 - 1, v2 - 1};
        faces.push_back(Face(3, v, NULL, n - 1));
    }

    void add_face_3vtn(std::string &line) {
        int v0, v1, v2, t0, t1, t2, n;
        sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &v0, &t0, &n, &v1, &t1, &n, &v2, &t2, &n);
        int *v = new int[3]{v0 - 1, v1 - 1, v2 - 1};
        int *t = new int[3]{t0 - 1, t1 - 1, t2 - 1};
        faces.push_back(Face(3, v, t, n - 1));
    }

    void add_face_4v(std::string &line) {
        int v0, v1, v2, v3;
        sscanf(line.c_str(), "f %d %d %d %d", &v0, &v1, &v2, &v3);
        int *v = new int[4]{v0 - 1, v1 - 1, v2 - 1, v3 - 1};
        faces.push_back(Face(4, v, NULL));
    }

    void add_face_4vt(std::string &line) {
        int v0, v1, v2, v3, t0, t1, t2, t3;
        sscanf(line.c_str(), "f %d/%d %d/%d %d/%d %d/%d", &v0, &t0, &v1, &t1, &v2, &t2, &v3, &t3);
        int *v = new int[4]{v0 - 1, v1 - 1, v2 - 1, v3 - 1};
        int *t = new int[4]{t0 - 1, t1 - 1, t2 - 1, t3 - 1};
        faces.push_back(Face(4, v, t));
    }

    void add_face_4vn(std::string &line) {
        int v0, v1, v2, v3, n;
        sscanf(line.c_str(), "f %d//%d %d//%d %d//%d %d//%d", &v0, &n, &v1, &n, &v2, &n, &v3, &n);
        int *v = new int[4]{v0 - 1, v1 - 1, v2 - 1, v3 - 1};
        faces.push_back(Face(4, v, NULL, n - 1));
    }

    void add_face_4vtn(std::string &line) {
        int v0, v1, v2, v3, t0, t1, t2, t3, n;
        sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &v0, &t0, &n, &v1, &t1, &n, &v2, &t2, &n, &v3,
               &t3, &n);
        int *v = new int[4]{v0 - 1, v1 - 1, v2 - 1, v3 - 1};
        int *t = new int[4]{t0 - 1, t1 - 1, t2 - 1, t3 - 1};
        faces.push_back(Face(4, v, t, n - 1));
    }

  public:
    void load(const char *filename) {
        has_texcoord = has_normal = false;

        add_face_3 = NULL;
        add_face_4 = NULL;

        std::string line;
        std::vector<std::string> lines;
        std::ifstream in(filename);
        if (!in.is_open()) {
            printf("Cannot load model %s\n", filename);
            return;
        }

        while (!in.eof()) {
            std::getline(in, line);
            lines.push_back(line);
        }
        in.close();

        float a, b, c;
        char str[40];

        for (std::string &line : lines) {
            if (line[0] == 'v') {
                if (line[1] == ' ') {
                    sscanf(line.c_str(), "v %f %f %f", &a, &b, &c);
                    vertices.push_back(new float[3]{a, b, c});
                } else if (line[1] == 't') {
                    sscanf(line.c_str(), "vt %f %f", &a, &b);
                    texcoords.push_back(new float[2]{a, b});
                } else {
                    sscanf(line.c_str(), "vn %f %f %f", &a, &b, &c);
                    normals.push_back(new float[3]{a, b, c});
                }
            } else if (line[0] == 'f') {
                int edge = count_char(line, ' ');
                if (add_face_3 == NULL) {
                    int count_slash = count_char(line, '/');
                    if (count_slash == 0) {
                        add_face_3 = &add_face_3v;
                        add_face_4 = &add_face_4v;
                    } else if (count_slash == edge) {
                        add_face_3 = &add_face_3vt;
                        add_face_4 = &add_face_4vt;
                    } else if (count_slash == edge * 2) {
                        has_normal = true;
                        if (has_double_slash(line)) {
                            add_face_3 = &add_face_3vn;
                            add_face_4 = &add_face_4vn;
                        } else {
                            add_face_3 = &add_face_3vtn;
                            add_face_4 = &add_face_4vtn;
                        }
                    }
                }
                switch (edge) {
                case 3:
                    (this->*add_face_3)(line);
                    break;
                case 4:
                    (this->*add_face_4)(line);
                    break;
                }
            } else if (line[0] == 't') {
                Image img;
                sscanf(line.c_str(), "t %s", &str);
                Load_Texture_Swap(&img, str);
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.w, img.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.img);
                glBindTexture(GL_TEXTURE_2D, 0);
                Delete_Image(&img);
                has_texcoord = true;
            } else if (line[0] == 'm' && line[1] == 't') {
                std::string tmp = filename;
                std::string material = "";
                int n = tmp.find_last_of('/') + 1;
                if (n > 0)
                    material = tmp.substr(0, n);
                sscanf(line.c_str(), "mtllib %s", &str);
                material += str;
                load_material(material.c_str());
            } else if (line[0] == 'u' && line[1] == 's') {
                sscanf(line.c_str(), "usemtl %s", &str);
                std::string material = str;
                if (map_material.find(material) != map_material.end())
                    faces.push_back(Face(-1, NULL, NULL, map_material[material]));
            }
        }

        list = glGenLists(1);
        glNewList(list, GL_COMPILE);
        if (has_texcoord)
            glBindTexture(GL_TEXTURE_2D, texture);
        if (!has_normal)
            glDisable(GL_LIGHTING);
        for (Face &face : faces) {
            if (face.edge == -1) {
                glLightfv(GL_LIGHT0, GL_AMBIENT, materials[face.normal].ambient);
                glLightfv(GL_LIGHT0, GL_DIFFUSE, materials[face.normal].diffuse);
                glLightfv(GL_LIGHT0, GL_SPECULAR, materials[face.normal].specular);
                continue;
            }
            if (has_normal)
                glNormal3fv(normals[face.normal]);
            if (has_texcoord) {
                glBegin(GL_POLYGON);
                for (int i = 0; i < face.edge; i++) {
                    glTexCoord2fv(texcoords[face.texcoords[i]]);
                    glVertex3fv(vertices[face.vertices[i]]);
                }
                glEnd();
            } else {
                glBegin(GL_POLYGON);
                for (int i = 0; i < face.edge; i++)
                    glVertex3fv(vertices[face.vertices[i]]);
                glEnd();
            }
        }
        if (!has_normal)
            glEnable(GL_LIGHTING);
        glEndList();

        printf("Model: %s\n", filename);
        printf("Vertices: %d\n", vertices.size());
        printf("Texcoords: %d\n", texcoords.size());
        printf("Normals: %d\n", normals.size());
        printf("Faces: %d\n", faces.size());

        for (Material &material : materials) {
            delete material.ambient;
            delete material.diffuse;
            delete material.specular;
        }

        materials.clear();
        map_material.clear();

        for (float *f : vertices)
            delete f;
        vertices.clear();
        for (float *f : texcoords)
            delete f;
        texcoords.clear();
        for (float *f : normals)
            delete f;
        normals.clear();
        faces.clear();
    }

    void draw() { glCallList(list); }
};

Model model;

float angle = 0.0f;

GLfloat light_pos[] = {-300.0f, 50.0f, 300.00f, 1.0f};
const GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};

bool render = false;
int fps = 0;
int timer = 0, timer2 = 0;
int start = clock(), end;

float t=0.0f;
float radius=100.0f;

void Display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    light_pos[0]=30*sin(t)+10;
    light_pos[2]=30*cos(t);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
//    glTranslatef(0.0f, 0.0f, 0.0f);
//    glRotatef(angle, 0.0f, 1.0f, 0.0f);
//        glutSolidSphere(20, 40,40);
	glLoadIdentity();
	glTranslatef(-20.0,0,0);
	glutSolidSphere(8, 40,40);
	glLoadIdentity();
	glTranslatef(15.0,0,0);
	glutSolidTeapot(10);
//    model.draw();
    glutSwapBuffers();
}

// void Idle() {
//     end = clock();
//     timer += end - start;
//     start = end;
//     if (timer >= 15) {
//         angle += 2.0f * timer / 15;
//         timer2 += timer;
//         if (timer2 >= 1000) {
//             timer2 = 0;
//             printf("%d\n", fps);
//             fps = 0;
//         }
//         timer = 0;
//         fps++;
//         glutPostRedisplay();
//     }
// }

void Timer(int value) {
	t+=0.02f;
//	glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluPerspective(20.0, 1.0, 10.0, 1000.0);
//    float camX = sin(t) * radius;
//	float camZ = cos(t) * radius;
//    gluLookAt(camX,0.0,camZ-200,0.0,0.0,-200.0,0.0,1.0,0.0);
//    glTranslatef(0.0f, 0.0f, -200.0f);
//    glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
//    glMatrixMode(GL_MODELVIEW);
    angle += 1.0f;
    glutPostRedisplay();
    glutTimerFunc(INTERVAL, Timer, 0);
}

void Init_GL() {
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_FRONT);
    // glShadeModel(GL_SMOOTH);
    // glShadeModel(GL_FLAT);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialf(GL_FRONT, GL_SHININESS, 128.0f);

    // glEnable(GL_COLOR_MATERIAL);
    // glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // gluOrtho2D(-1.0, 1.0, 1.0, -1.0);
    gluPerspective(20.0, 1.0, 1.0, 1000.0);
//    float camX = cos(t) * radius;
//	float camZ = sin(t) * radius;
//    gluLookAt(camX,0.0,camZ,0.0,0.0,-200.0,0.0,1.0,0.0);
//     glOrtho(-1.0, 1.0, -1.0, 1.0, 1.0, 1000.0);
    glTranslatef(0.0f, 0.0f, -200.0f);
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

    model.load(MODEL_NAME);
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

    // glutIdleFunc(Idle);
    Init_GL();
    glutDisplayFunc(Display);
    glutTimerFunc(0, Timer, 0);
    glutMainLoop();
    return 0;
}
