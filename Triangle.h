#ifndef TRIANGLE_H
#define TRIANGLE_H


#include "Ray.h"
#include "Vector3D.h"


#define EPSILON 0.001


class Triangle {
   public:
      Triangle();
      ~Triangle();

      double Intersect(Ray *ray);
   public:
      Vector3D c1, c2, c3;
};


#endif // TRIANGLE_H
