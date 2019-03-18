#include "myopengl.h"
#include<iostream>
myopenGL::myopenGL(QWidget *parent): QOpenGLWidget(parent), QOpenGLFunctions()
{

}
void myopenGL::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0,0,0,1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    //cout<<glGetString(GL_VERSION)<<endl;
}
void myopenGL::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBegin(GL_TRIANGLES);
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(-5, -5, 0);
        glColor3f(0.0, 1.0, 0.0);
        glVertex3f( 5, -5, 0);
        glColor3f(0.0, 0.0, 1.0);
        glVertex3f( 0.0,  5, 0);
    glEnd();
}
void myopenGL::resizeGL(int width, int height)
{
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-width/2,width/2,-height/2,height/2,-1,1);
    glMatrixMode(GL_MODELVIEW);
}
