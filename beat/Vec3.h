#ifndef BEAT_VEC3_H
#define BEAT_VEC3_H

#include <cmath>

namespace Grasp {
    const double MY_PI = 3.14159265;
    const double MY_2PI = MY_PI * 2.0;
    const double MY_PI_HALF = MY_PI * 0.5;
    const double MY_PI_15 = MY_PI * 1.5;

/**
@class   Vec3
@brief   3-х мерный вектор
*/
class Vec3 
{
public:
    /**
    конструктор без параметров - 3-х мерный нулевой вектор
    */
    Vec3() {
      x = 0.0;
      y = 0.0;
      z = 0.0;
    }

    /**
    конструктор с параметрами для инициализации вектора 
    */   
    Vec3(double x0, double y0 = 0, double z0 = 0) {
      x = x0;
      y = y0;
      z = z0;
    }

    /**
    конструктор с параметрами для инициализации вектора
    */ 
    Vec3(double dx[3]) {
      x = dx[0];
      y = dx[1];
      z = dx[2];
    }

    /**
    копирование данных
    */
    void Copy(double x0, double y0, double z0) {
      x = x0;
      y = y0;
      z = z0;
    }

    /**
    копирование данных из массива
    */
    void Copy(double dx[3]) {
      x = dx[0];
      y = dx[1];
      z = dx[2];
    }

    /**
    копирование данных из другого вектора
    */   
    void Copy(Vec3& v) {
      x = v.x;
      y = v.y;
      z = v.z;
    }

    /**
    прибавление вектора, параметры - его компоненты
    */   
    void Add(double x0, double y0, double z0) {
      x += x0;
      y += y0;
      z += z0;
    }

    /**
    прибавление вектора
    */
    void Add(Vec3& v) {
        x += v.x;
        y += v.y;
        z += v.z;
    }

    /**
    вычитание вектора
    */
    void Sub(Vec3& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
    }

    /**
    сложение 2-х векторов, результат записывается в res
    */   
    void Sum(Vec3& v, Vec3& res) {
      res.x = x + v.x;
      res.y = y + v.y;
      res.z = z + v.z;
    }

    /**
    векторное произведение, результат в v3
    */
    static void Product(Vec3& v1, Vec3& v2, Vec3& v3) {
      v3.z = v1.x * v2.y - v1.y * v2.x;
      v3.x = v1.y * v2.z - v1.z * v2.y;
      v3.y = v1.z * v2.x - v1.x * v2.z;
    }

    /**
    длина вектора
    */  
    double Length() { return sqrt(x * x + y * y + z * z); }

    /**
    расстояние между 2-мя векторами
    */    
    double Distance(double x1, double y1, double z1) {
      return sqrt((x - x1) * (x - x1) + (y - y1) * (y - y1) + (z - z1) * (z - z1));
    }

    /**
    расстояние между 2-мя векторами
    */
    double Distance(Vec3& v) {
      return sqrt((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z));
    }

    /**
    нормализация вектора, длина = 1
    */   
    double Normalize() {
      double len = Length();
      if (len != 0) {
        x /= len;
        y /= len;
        z /= len;
      }
      return len;
    }
    
    /**
    скалярное произведение
    */
    double ScalarProduct(Vec3& v1) {
      // len(a) * len(b) * cos(fi)
      return (x * v1.x + y * v1.y + z * v1.z);
    }

    /**
    масштабирование
    */
    void Scale(double p) {
        x *= p;
        y *= p;
        z *= p;
    }

    /**
    проверка на нулевой вектор
    */
    bool IsZero() {
        return (x == 0.0 && y == 0.0 && z == 0.0 );
    }

    /**
    получить x
    */
    double GetX() { return x; }
    /**
    получить y
    */
    double GetY() { return y; }
    /**
    получить z
    */
    double GetZ() { return z; }

    /**
    установить x
    */
    void SetX(double _x) { x = _x; }
    /**
    установить y
    */
    void SetY(double _y) { y = _y; }
    /**
    установить z
    */
    void SetZ(double _z) { z = _z; }

protected:
private:
  double x, y, z; ///< координаты вектора
};

} // namespace Grasp
#endif