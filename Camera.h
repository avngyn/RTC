#ifndef CAMERA_H
#define CAMERA_H


#include <list>

#include "Ray.h"
#include "Vector3D.h"


struct Plane {
   Vector3D normal;
   double d;
};


class Camera {
   public:
      Camera(int width, int height);
      ~Camera();

      void GenerateRays(std::list<Ray> *rays);
   public:
      Vector3D position;
      Vector3D lookat;

      Plane planes[6];
   private:
      int width, height;
      double aspect_ratio;

      double l, r, t, b;
      double rlw, tbh;
};


#endif // CAMERA_H
