#ifndef RAY_H
#define RAY_H


#include "Mesh.h"
#include "Vector3D.h"


class Ray {
   public:
      Ray();
      ~Ray();
   public:
      Vector3D pos, dir;
      unsigned int pixel;
      double dist;
      unsigned int hit_obj;
      Face *hit_tri;
};


#endif // RAY_H
