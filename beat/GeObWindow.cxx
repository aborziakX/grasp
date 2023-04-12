﻿#include "GeObWindow.h"

namespace Grasp {

GeObWindow::GeObWindow()
{
    lasttime = 0.0;
    vecCam.Copy(0.0, 0.0, 5.0); // позиция камеры
    vecLook.Copy(0.0, 0.0, 0.0); // камера смотрит сюда
    vecTop.Copy(0.0, 1.0, 0.0); ///< вертикальный вектор камеры
    //test
    SetPolar(distance, azimut, elevation);
}

GeObWindow::~GeObWindow() 
{
  for (int i = 0; i < vecGeOb.size(); i++) {
    if (vecGeOb[i] == NULL)
      continue;
    delete vecGeOb[i];
    vecGeOb[i] = NULL;
  }
}

/*// курсор
int GeObWindow::handle(int e) {
  switch (e) {
    case FL_ENTER:
      cursor(FL_CURSOR_CROSS);
      break;
    case FL_LEAVE:
      cursor(FL_CURSOR_DEFAULT);
      break;
  }
  return Fl_Gl_Window::handle(e);
}*/

void test1()
{
    float m[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, m);
    //identity
    //[1 0 0 0]
    //[0 1 0 0]
    //[0 1 0 0]
    //[0 0 0 1]
    return;
}
void test2()
{
    float m[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, m);
    return;
}

// рисовать
void GeObWindow::Draw() {
    // Очистка буфера цвета и глубины
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // обнулить трансформацию
    glLoadIdentity();
    test1();
    glPushMatrix();

    // установить камеру
    double x = vecCam.GetX(), y = vecCam.GetY(), z = vecCam.GetZ();
    double x_l = vecLook.GetX(), y_l = vecLook.GetY(), z_l = vecLook.GetZ();
    double x_tp = vecTop.GetX(), y_tp = vecTop.GetY(), z_tp = vecTop.GetZ();

    gluLookAt(x, y, z,
        x_l, y_l, z_l,
        x_tp, y_tp, z_tp);
    test2();

    Vec3 vecCamDir(x_l - x, y_l - y, z_l - z);
    vecCamDir.Normalize();

    /*// нарисуем "землю"
    glColor3f(0.9f, 0.9f, 0.9f);
    glBegin(GL_QUADS); // полигон с коондинатами
    glVertex3f(-100.0f, 0.0f, -100.0f);
    glVertex3f(-100.0f, 0.0f, 100.0f);
    glVertex3f(100.0f, 0.5f, 100.0f);
    glVertex3f(100.0f, 0.0f, -100.0f);
    glEnd();*/

    /*glRotatef(float(dRotateZ * 360), 0, 0, 1);
    glRotatef(float(dRotateX * 360), 1, 0, 0);
    glRotatef(float(dRotateY * 360), 0, 1, 0);
    glTranslatef(float(dShiftX), float(dShiftY), float(dShiftZ));
    glScalef(float(size), float(size), float(size));*/

    // оси
    glBegin(GL_LINE_LOOP);
    glColor3ub(255, 0, 0);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(3.0f, 0.0f, 0.0f);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3ub(0, 255, 0);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 3.0f, 0.0f);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3ub(0, 0, 255);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 3.0f);
    glEnd();

    // рисовать все GeOb здесь
    for (int i = 0; i < vecGeOb.size(); i++) {
        if (vecGeOb[i] == NULL)
            continue;
        vecGeOb[i]->Draw(vecCamDir);
    }

    glPopMatrix();

    // рисовать заголовок
    gl_color(FL_GRAY);
    glDisable(GL_DEPTH_TEST);
    gl_font(FL_HELVETICA_BOLD, 16);
    gl_draw(wire ? "Cube: wire" : "Cube: flat", -4.5f, -4.5f);
    glEnable(GL_DEPTH_TEST);

    // если OpenGL graphics driver установлен, дать ему шанс
    // рисовать виджеты
    //if(win_glut != NULL) win_glut->display();

    int mode = glutGet(GLUT_INIT_DISPLAY_MODE);
    if ((mode & GLUT_DOUBLE) > 0)
        glutSwapBuffers();
    else glutPostRedisplay();
}

// рисовать old
/*void GeObWindow::drawOld() {
    if (!valid()) {
        glLoadIdentity();
        glViewport(0, 0, pixel_w(), pixel_h());
        glEnable(GL_DEPTH_TEST);
        glFrustum(-1, 1, -1, 1, 2, 10000);
        glTranslatef(0, 0, -dCameraZ); //-10); // translate coordinate system from camera
        glClearColor(0.2f, 0.4f, 0.2f, 0);
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    glRotatef(float(dRotateZ * 360), 0, 0, 1);
    glRotatef(float(dRotateX * 360), 1, 0, 0);
    glRotatef(float(dRotateY * 360), 0, 1, 0);
    glTranslatef(float(dShiftX), float(dShiftY), float(dShiftZ));
    glScalef(float(size), float(size), float(size));
    // рисовать все GeOb здесь
    for (int i = 0; i < vecGeOb.size(); i++) {
        if (vecGeOb[i] == NULL)
            continue;
        vecGeOb[i]->Draw();
    }

    glPopMatrix();

    // рисовать заголовок
    gl_color(FL_GRAY);
    glDisable(GL_DEPTH_TEST);
    gl_font(FL_HELVETICA_BOLD, 16);
    gl_draw(wire ? "Cube: wire" : "Cube: flat", -4.5f, -4.5f);
    glEnable(GL_DEPTH_TEST);

    // если OpenGL graphics driver установлен, дать ему шанс
    // рисовать виджеты
    Fl_Gl_Window::draw();
}*/

 /*// set zCam
void GeObWindow::SetZCam(float f) {
  if (f == zCam)
    return;
  // сдвиг накапливается, поэтому нужен сброс
  glTranslatef(0, 0, zCam - f); // сбросить текущий сдвиг и установить новый
  zCam = f;
}*/

// получить полярные координаты
void GeObWindow::GetPolar(double& _distance, double& _azimut, double& _elevation)
{
    _distance = distance;
    _azimut = azimut;
    _elevation = elevation;
};

// установить полярные координаты
void GeObWindow::SetPolar(double _distance, double _azimut, double _elevation)
{
    distance = _distance;
    azimut = _azimut;
    elevation = _elevation;

    if (azimut < 0.0) azimut += (float)MY_2PI;
    else if (azimut > MY_2PI) azimut -= (float)MY_2PI;
    if (elevation < 0.0) elevation += (float)MY_2PI;
    else if (elevation > MY_2PI) elevation -= (float)MY_2PI;

    // найти vecCam (позиция камеры) и  vecTop (вертикальный вектор камеры)
    // vecLook (камера смотрит сюда) не меняется

    // в уме сдвинем vecLook в начало координат
    // Z на поляную звезду (север), X - экватор нулевой меридиан, Y = Z prod X
    double x = distance, y = 0.0, z = 0.0;

    // вращать вокруг Y на -elevation
    double y1 = y;
    double z1 = z * cos(-elevation) - x * sin(-elevation);
    double x1 = z * sin(-elevation) + x * cos(-elevation);

    // вращать вокруг Z на azimut
    double x2 = x1 * cos(azimut) - y1 * sin(azimut);
    double y2 = x1 * sin(azimut) + y1 * cos(azimut);
    double z2 = z1;

    vecCam.Copy(x2, y2, z2);

    Vec3 v2, vNorm;
    v2.Copy(x2, y2, 0); // на экваторе

    /*if (!vecLook.IsZero())
    {   // сдвиг на vecLook обратно
        vecCam.Add(vecLook);
        v2.Add(vecLook);
    }*/

    // найти vecTop
    if (z2 == 0)
    {
        vecTop.Copy(0, 0, 1);
    }
    else
    {
        Vec3::Product(v2, vecCam, vNorm); // нормаль к поскости элевации
        if( (elevation > 0.0 && elevation < MY_PI_HALF) || (elevation > MY_PI && elevation < MY_PI_15) )
            Vec3::Product(vNorm, vecCam, vecTop);
        else Vec3::Product(vecCam, vNorm, vecTop); //обратный порядок, чтоб камера смотрела вверх
    }

    double sc = vecCam.ScalarProduct(vecTop);
    if (!vecLook.IsZero())
    {   // сдвиг на vecLook обратно
        vecCam.Add(vecLook);
    }

    vecTop.Normalize();

    //test
    double di = vecCam.Distance(vecLook);
    if (abs(di - distance) > 0.01 || abs(sc) > 0.01 )
    {
        int k = 0;
    }
};

// вычислить полярные координаты по vecCam, vecLook, vecTop
void GeObWindow::CalcPolar(double& _distance, double& _azimut, double& _elevation)
{
    double dx = (vecLook.GetX() - vecCam.GetX());
    double dy = (vecLook.GetY() - vecCam.GetY());
    double dz = (vecLook.GetZ() - vecCam.GetZ());

    distance = sqrt(dx * dx + dy * dy + dz * dz);
    if (distance != 0.0)
    {
        if (dx != 0.0 || dy != 0.0)
        {
            elevation = -asin(dz / distance);
            if (dx != 0.0)
                azimut = atan(dy / dx);
            else azimut = 0.0;
        }
        else
        {   //на поляную звезду или на юг строго
            elevation = dz > 0.0 ? MY_PI_HALF : -MY_PI_HALF;
            azimut = 0.0;
        }
    }
    else
    {
        elevation = 0.0;
        azimut = 0.0;
    }

    _distance = distance;
    _azimut = azimut;
    _elevation = elevation;
}

// gluLookAt2 метод позиционирования камеры
// helpers for gluLookAt2
void normalize(float forward[3])
{
    float len = sqrt(forward[0] * forward[0] + forward[1] * forward[1] + forward[2] * forward[2]);
    if (len != 0.0f)
    {
        forward[0] /= len;
        forward[1] /= len;
        forward[2] /= len;
    }
}
void cross(float forward[3], float up[3], float side[3])
{
    side[2] = forward[0] * up[1] - forward[1] * up[0];
    side[0] = forward[1] * up[2] - forward[2] * up[1];
    side[1] = forward[2] * up[0] - forward[0] * up[2];
}
void __gluMakeIdentityf(GLfloat * m)
{
    m[0] = 1.0f;
    m[1] = 0.0f;
    m[2] = 0.0f;
    m[3] = 0.0f;

    m[4] = 0.0f;
    m[5] = 1.0f;
    m[6] = 0.0f;
    m[7] = 0.0f;

    m[8] = 0.0f;
    m[9] = 0.0f;
    m[10] = 1.0f;
    m[11] = 0.0f;

    m[12] = 0.0f;
    m[13] = 0.0f;
    m[14] = 0.0f;
    m[15] = 1.0f;
}

//from http://www.mesa3d.org
void GeObWindow::gluLookAt2(GLdouble eyex, GLdouble eyey, GLdouble eyez, 
    GLdouble centerx, GLdouble centery, GLdouble centerz, 
    GLdouble upx, GLdouble upy, GLdouble upz)
{
    float forward[3], side[3], up[3];
    GLfloat m[4][4];

    forward[0] = (float)(centerx - eyex);
    forward[1] = (float)(centery - eyey);
    forward[2] = (float)(centerz - eyez);

    up[0] = (float)upx;
    up[1] = (float)upy;
    up[2] = (float)upz;

    normalize(forward);
    
    // Side = forward x up 
    cross(forward, up, side);
    normalize(side);

    // Recompute up as: up = side x forward 
    cross(side, forward, up);

    __gluMakeIdentityf(&m[0][0]);
    m[0][0] = side[0];
    m[1][0] = side[1];
    m[2][0] = side[2];

    m[0][1] = up[0];
    m[1][1] = up[1];
    m[2][1] = up[2];

    m[0][2] = -forward[0];
    m[1][2] = -forward[1];
    m[2][2] = -forward[2];

    glMultMatrixf(&m[0][0]);
    glTranslated(-eyex, -eyey, -eyez);
}

} // namespace Grasp