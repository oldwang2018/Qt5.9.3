#ifndef MYOPENGL_H
#define MYOPENGL_H

#include <QWidget>
#include<GL/gl.h>
#include<GL/glu.h>
#include<QOpenGLWidget>
#include"QOpenGLFunctions"
class myopenGL:public QOpenGLWidget,protected QOpenGLFunctions
{
public:
    myopenGL(QWidget *parent = 0);
public:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width,int height) Q_DECL_OVERRIDE;
signals:

public slots:


};

#endif // MYOPENGL_H
