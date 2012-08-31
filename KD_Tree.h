#ifndef KD_TREE_H
#define KD_TREE_H


#include <vector>

#include "Camera.h"
#include "Mesh.h"
#include "Ray.h"
#include "Triangle.h"
#include "Vector3D.h"


struct KD_Node {
   KD_Node *left, *right;
   Face *face;
   int axis;
   Vector3D min, max;
   bool inactive, skip;
   Vector3D sp;
   double sp_radius;
   KD_Node() { left = 0; right = 0; face = 0; axis = 0; inactive = false; skip = false; }
};


class KD_Tree {
   public:
      KD_Tree();
      ~KD_Tree();

      void Build();
      void Cull(Camera *cam, KD_Node *node);
      void Intersect(Ray *ray, KD_Node *node, bool &hit, bool optimize);
      void Delete(KD_Node *node);
   public:
      KD_Node *root;
      Mesh *object;
      Vector3D position;
};


#endif
