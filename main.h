#ifndef MAIN_H
#define MAIN_H

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

class CubeObject {
  public:
    float pos_x, pos_y, pos_z;
    float rotate_x, rotate_y, rotate_z;
    float scale_x, scale_y, scale_z;

    float n[6][3] = {{-1.0, 0.0, 0.0}, {1.0, 0.0, 0.0},  {0.0, -1.0, 0.0},
                     {0.0, 1.0, 0.0},  {0.0, 0.0, -1.0}, {0.0, 0.0, 1.0}};

    CubeObject(float pos_x = 0.0f, float pos_y = 0.0f, float pos_z = 0.0f, float rotate_x = 0.0f,
               float rotate_y = 0.0f, float rotate_z = 0.0f, float scale_x = 1.0f,
               float scale_y = 1.0f, float scale_z = 1.0f) {
        this->pos_x = pos_x;
        this->pos_y = pos_y;
        this->pos_z = pos_z;
        this->rotate_x = rotate_x;
        this->rotate_y = rotate_y;
        this->rotate_z = rotate_z;
        this->scale_x = scale_x;
        this->scale_y = scale_y;
        this->scale_z = scale_z;
    }

    void draw() {
        glLoadIdentity();
        glTranslatef(pos_x, pos_y, pos_z);
        // glRotatef(360.0f, rotate_x / 360.0f, rotate_y / 360.0f, rotate_z / 360.0f);

        glRotatef(360.0f, 0.0f, 0.0f, 0.1f);
        float vlbf[] = {-scale_x, -scale_y, -scale_z}; // left, bottom, front
        float vltf[] = {-scale_x, scale_y, -scale_z};
        float vlbb[] = {-scale_x, -scale_y, scale_z};
        float vltb[] = {-scale_x, scale_y, scale_z};
        float vrbf[] = {scale_x, -scale_y, -scale_z};
        float vrtf[] = {scale_x, scale_y, -scale_z};
        float vrbb[] = {scale_x, -scale_y, scale_z};
        float vrtb[] = {scale_x, scale_y, scale_z};
        float *faces[6][4] = {{vlbb, vlbf, vltf, vltb}, {vrbb, vrbf, vrtf, vrtb},
                              {vlbf, vrbf, vrbb, vlbb}, {vltf, vrtf, vrtb, vltb},
                              {vlbf, vrbf, vrtf, vltf}, {vlbb, vrbb, vrtb, vltb}};

        for (int i = 0; i < 6; i++) {
            glBegin(GL_QUADS);
            glNormal3fv(&n[i][0]);
            glVertex3fv(faces[i][0]);
            glVertex3fv(faces[i][1]);
            glVertex3fv(faces[i][2]);
            glVertex3fv(faces[i][3]);
            glEnd();
        }
    }
};

#define RAD 57.295780f

class Cube {
  public:
    float min_scale_y, max_scale_y;
    float pos_x, pos_y, pos_z;
    float scale_x, scale_y, scale_z;
    float vy;
    bool is_spring;
    int timer;
    int stt;
    float angle, b, save;
    float n[6][3] = {{-1.0, 0.0, 0.0}, {1.0, 0.0, 0.0},  {0.0, -1.0, 0.0},
                     {0.0, 1.0, 0.0},  {0.0, 0.0, -1.0}, {0.0, 0.0, 1.0}};

    Cube() {
        min_scale_y = 0.3f;
        max_scale_y = 1.2f;
        scale_y = (min_scale_y + max_scale_y) / 2;
        scale_x = 1.4f - scale_y;
        scale_z = 0.6f;

        pos_x = 0.0f;
        pos_y = scale_y / 2;
        pos_z = 0.0f;

        vy = 0.0f;
        is_spring = false;
        stt = 0;
    }

    void draw() {
        // glLoadIdentity();
        // glTranslatef(-1.5f, 0.0f, 0.0f);
        // float color[] = {0.7f, 0.3f, 0.0f};
        // glColor3fv(color);
        // glTranslatef(pos_x, pos_y, pos_z);
        float vlbf[] = {-scale_x, -scale_y, -scale_z}; // left, bottom, front
        float vltf[] = {-scale_x, scale_y, -scale_z};
        float vlbb[] = {-scale_x, -scale_y, scale_z};
        float vltb[] = {-scale_x, scale_y, scale_z};
        float vrbf[] = {scale_x, -scale_y, -scale_z};
        float vrtf[] = {scale_x, scale_y, -scale_z};
        float vrbb[] = {scale_x, -scale_y, scale_z};
        float vrtb[] = {scale_x, scale_y, scale_z};
        float *faces[6][4] = {{vlbb, vlbf, vltf, vltb}, {vrbb, vrbf, vrtf, vrtb},
                              {vlbf, vrbf, vrbb, vlbb}, {vltf, vrtf, vrtb, vltb},
                              {vlbf, vrbf, vrtf, vltf}, {vlbb, vrbb, vrtb, vltb}};

        glLoadIdentity();
        float color[] = {0.7f, 0.3f, 0.0f};
        glColor3fv(color);
        glTranslatef(pos_x, pos_y, pos_z);
        for (int i = 0; i < 6; i++) {
            glBegin(GL_QUADS);
            glNormal3fv(&n[i][0]);
            glVertex3fv(faces[i][0]);
            glVertex3fv(faces[i][1]);
            glVertex3fv(faces[i][2]);
            glVertex3fv(faces[i][3]);
            glEnd();
        }

        // glLoadIdentity();
        // glTranslatef(-1.5f, 0.0f, 0.0f);
        // float color1[] = {0.0f, 0.7f, 0.3f};
        // glColor3fv(color1);
        // glTranslatef(pos_x, pos_y, pos_z);
        // for (int i = 0; i < 6; i++) {
        //     glBegin(GL_QUADS);
        //     glNormal3fv(&n[i][0]);
        //     glVertex3fv(faces[i][0]);
        //     glVertex3fv(faces[i][1]);
        //     glVertex3fv(faces[i][2]);
        //     glVertex3fv(faces[i][3]);
        //     glEnd();
        // }

        // glLoadIdentity();
        // glTranslatef(1.5f, 0.0f, 0.0f);
        // float color2[] = {0.3f, 0.0f, 0.7f};
        // glColor3fv(color2);
        // glTranslatef(pos_x, pos_y, pos_z);
        // for (int i = 0; i < 6; i++) {
        //     glBegin(GL_QUADS);
        //     glNormal3fv(&n[i][0]);
        //     glVertex3fv(faces[i][0]);
        //     glVertex3fv(faces[i][1]);
        //     glVertex3fv(faces[i][2]);
        //     glVertex3fv(faces[i][3]);
        //     glEnd();
        // }
    }

    void move_up() {
        if (stt != 2) {
            stt = 0;
            vy = 0.03f;
            is_spring = false;
        }
    }

    void move_down() {
        if (stt != 1) {
            stt = 0;
            vy = -0.03f;
            is_spring = false;
        }
    }

    void update() {
        // if (is_spring) {
        //     timer++;
        //     if (timer == 10) {
        //         timer = 0;
        //         vy = -vy;
        //     }
        //     vy *= 0.97f;
        // }

        // if (is_spring) {
        //     angle += 20.0f;
        //     scale_y = save + b * cos(angle / RAD);
        //     b *= 0.97f;
        //     if (b < 0.001f)
        //         is_spring = false;
        //     scale_x = 1.4f - scale_y;
        //     pos_y = scale_y / 2;
        // }

        if (is_spring) {
            float a = -(scale_y - save) * 0.1f;
            vy += a;
            vy *= 0.97f;
            scale_y += vy;
            // if ()
            //     is_spring = false;
            scale_x = 1.4f - scale_y;
            pos_y = scale_y / 2;
        }

        if (stt == 0) {
            if (vy < -0.001f) {
                scale_y += vy;
                if (!is_spring && scale_y < min_scale_y) {
                    scale_y = min_scale_y;
                    stt = 1;
                    spring();
                }
                scale_x = 1.4f - scale_y;
                pos_y = scale_y / 2;
            } else if (vy > 0.001f) {
                scale_y += vy;
                if (!is_spring && scale_y > max_scale_y) {
                    scale_y = max_scale_y;
                    stt = 2;
                    spring();
                }
                scale_x = 1.4f - scale_y;
                pos_y = scale_y / 2;
            }
        }
    }

    void spring() {
        if (vy < -0.02f || vy > 0.02f) {
            is_spring = true;
            timer = 0;
            b = 0.03f;
            angle = vy < 0 ? 180.0f : 0.0f;
            save = scale_y;
            scale_y += vy;
            vy = 0.0f;
        }
    }
};

Cube cube;

// CubeObject cubeObject(0, 0, 0, 30, 30, 30);

// including all referenced .cpp files, you don't need to compile all of them
#include "afunc.cpp"
#include "init.cpp"

#endif
