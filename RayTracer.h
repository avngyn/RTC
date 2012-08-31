#ifndef RAYTRACER_H
#define RAYTRACER_H


#include <iostream>
#include <list>
#include <vector>

#include "Camera.h"
#include "KD_Tree.h"
#include "Light.h"
#include "OBJ.h"
#include "Ray.h"
#include "TGA.h"
#include "Triangle.h"
#include "Vector3D.h"


#define SCREEN_WIDTH 200
#define SCREEN_HEIGHT 200

#define FAR_PLANE 10000.0


class RayTracer {
   public:
      RayTracer();
      ~RayTracer();
      
      bool Initialize();
      bool Load(const char *filename);
      void Render();
      void Clean();
   private:
      void Cast();
      bool Intersect(Ray *ray, bool optimize);
      void Lighting(Ray *ray, Vector3D &color);
      void Output();
   private:
      Camera camera;
      TGA frame;

      std::list<Ray> cast_rays;
      std::list<Ray> light_rays;
      std::vector<Mesh *> objects;
      std::vector<Light> lights;

      std::vector<KD_Tree *> kdtrees;
};


#endif // RAYTRACER_H
