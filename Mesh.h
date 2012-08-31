#ifndef MESH_H
#define MESH_H


#include <vector>

#include "Vector3D.h"


#define EPSILON 0.001


struct Face {
   unsigned int vertex[3];
   unsigned int texcoord[3];
   unsigned int normal[3];
   Vector3D max, min;
};


class Mesh {
   public:
      Mesh();
      ~Mesh();

      void CalculateMaxMin();
      void CalculateNormals();
   public:
      std::vector<Vector3D *> vertices;
      std::vector<Face *> faces;
      std::vector<Vector3D *> normals;
      std::vector<Vector3D *> texcoords;
      std::vector<Vector3D *> centers;
      Vector3D position;
      Vector3D max, min, center;
      double radius;

      Vector3D pigment;
      double ambient, diffuse, specular;
      double reflection, refraction;
      double roughness;
      double ior;
};


#endif // MESH_H
