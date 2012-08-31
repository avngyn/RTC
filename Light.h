#ifndef LIGHT_H
#define LIGHT_H


#include "Vector3D.h"


class Light {
   public:
      Light();
      ~Light();
   public:
      Vector3D pos;
      Vector3D color;
};


#endif // LIGHT_H
