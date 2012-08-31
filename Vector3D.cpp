#include <math.h>


#include "Vector3D.h"


Vector3D::Vector3D() {
   x = 0.0;
   y = 0.0;
   z = 0.0;
}

Vector3D::Vector3D(double x, double y, double z) {
   this->x = x;
   this->y = y;
   this->z = z;
}

Vector3D::~Vector3D() {
}

double Vector3D::operator[](int i) {
   if (i == 0) {
      return x;
   }
   else if (i == 1) {
      return y;
   }
   else if (i == 2) {
      return z;
   }
   else {
      return 0;
   }
}

Vector3D & Vector3D::operator=(const Vector3D &v) {
   x = v.x;
   y = v.y;
   z = v.z;
   return *this;
}

Vector3D Vector3D::operator+(const Vector3D &v) const {
   Vector3D o;
   o.x = x+v.x;
   o.y = y+v.y;
   o.z = z+v.z;
   return o;
}

Vector3D Vector3D::operator-(const Vector3D &v) const {
   Vector3D o;
   o.x = x-v.x;
   o.y = y-v.y;
   o.z = z-v.z;
   return o;
}

Vector3D Vector3D::operator*(double s) const {
   Vector3D o;
   o.x = x*s;
   o.y = y*s;
   o.z = z*s;
   return o;
}

Vector3D Vector3D::operator/(double s) const {
   Vector3D o;
   o.x = x/s;
   o.y = y/s;
   o.z = z/s;
   return o;
}

void Vector3D::operator+=(const Vector3D &v) {
   x += v.x;
   y += v.y;
   z += v.z;
}

void Vector3D::operator-=(const Vector3D &v) {
   x -= v.x;
   y -= v.y;
   z -= v.z;
}

void Vector3D::operator*=(double s) {
   x *= s;
   y *= s;
   z *= s;
}

void Vector3D::operator/=(double s) {
   x /= s;
   y /= s;
   z /= s;
}

void Vector3D::Clear() {
   x = 0.0;
   y = 0.0;
   z = 0.0;
}

Vector3D Vector3D::Cross(const Vector3D &v) {
   Vector3D o;
   o.x = y*v.z-z*v.y;
   o.y = z*v.x-x*v.z;
   o.z = x*v.y-y*v.x;
   return o;
}

double Vector3D::Dot(const Vector3D &v) {
   double o;
   o = x*v.x + y*v.y + z*v.z;
   return o;
}

void Vector3D::Normalize() {
   double length = this->Length();
   x /= length;
   y /= length;
   z /= length;
}

double Vector3D::Length() {
   return sqrt(x*x + y*y + z*z);
}
