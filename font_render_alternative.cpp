#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <cstdio>
#include "ftgl.h"


void RenderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f , 0.2f , 0.2f);
    glRasterPos2f(1, 1);
    FTGLPixmapFont font("/usr/share/fonts/TTF/DejaVuSansMono.ttf");
    if(font.Error())
        printf("Font not found\n");
    font.FaceSize(30);
    font.Render("Привет, мир!");
    glRasterPos2f(0, 0);
    font.Render("Привет, мир!");
        glRasterPos2f(3, 3);

    font.Render("Привет, мир!");
        glRasterPos2f(4, 4);

    font.Render("Привет, мир!");
        glRasterPos2f(5, 5);

    glutSwapBuffers();
}

void ChangeSize(GLsizei w, GLsizei h) {
    if (h == 0)
        h = 1;
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat aspectRatio = (GLfloat)w/(GLfloat)h;
    if (w <= h)
       glOrtho(-10.0, 10.0, -10.0/aspectRatio, 10.0/aspectRatio, 1.0,-1.0);
    else
        glOrtho(-10.0*aspectRatio, 10.0*aspectRatio, -10.0, 10.0, 1.0,-1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void onTimer(int a) {
    glutPostRedisplay();
    glutTimerFunc(1,onTimer,1);
        //~ static int b;
    //~ if(b){glClearColor(1.0f, 1.0f, 0.8f, 1.0f); b=0;}
    //~ else {glClearColor(0.0f, 1.0f, 0.0f, 1.0f); b=1;}
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(800, 600);
    glutCreateWindow("hello");
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutTimerFunc(1,onTimer,1);

    glutMainLoop();
    return 0;
}
