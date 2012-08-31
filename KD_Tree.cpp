#include <iostream>

#include "KD_Tree.h"


KD_Tree::KD_Tree() {
   root = 0;
}

KD_Tree::~KD_Tree() {
}

void KD_Tree::Build() {
   KD_Node *node, *left_node, *right_node;
   std::vector<KD_Node *> node_stack;
   std::vector<std::vector<Face *>> build_stack;
   std::vector<Face *> *current_stack;
   std::vector<Face *> right_stack, left_stack;
   Face *f;
   int i, j, k, l, m;
   double x;
   double median;
   unsigned int num_faces;
   unsigned int a;

   Vector3D um;

   if (object == 0) {
      std::cout << "Need objects to build kd-tree.\n";
   }
      
   // Create root node
   root = new KD_Node();
   root->axis = 0;
   root->max = object->max;
   root->min = object->min;
   node_stack.push_back(root);

   build_stack.push_back(object->faces); // Assumes only one object
   while (!build_stack.empty()) {
      node = node_stack.back();
      current_stack = &build_stack.back();

      // Get number of faces in current stack
      num_faces = current_stack->size();

      // Calculate half of total number of faces
      if (num_faces & 1) {
         k = num_faces/2;
      }
      else {
         k = num_faces/2-1;
      }

      // Get median
      l = 0;
      m = num_faces-1;
      while (l < m) {
         x = (*current_stack)[k]->max[node->axis];
         i = l;
         j = m;
         do {
            while ((*current_stack)[i]->max[node->axis] < x) {
               i++;
            }
            while (x < (*current_stack)[j]->max[node->axis]) {
               j--;
            }
            if (i <= j) {
               f = (*current_stack)[i];
               (*current_stack)[i] = (*current_stack)[j];
               (*current_stack)[j] = f;
               i++;
               j--;
            }
         } while (i <= j);
         if (j < k) {
            l = i;
         }
         if (k < i) {
            m = j;
         }
      }
      median = (*current_stack)[k]->max[node->axis];
      node->face = (*current_stack)[k];

      // Add to stack
      left_stack.clear();
      right_stack.clear();
      for (a = 0; a < num_faces; a++) {
         if ((*current_stack)[a]->max[node->axis] <= median) {
            if (a != k) {
               // Left
               left_stack.push_back((*current_stack)[a]);
            }
         }
         else {
            // Right
            right_stack.push_back((*current_stack)[a]);
         }
      }

      // Pop current stack from build stack
      build_stack.pop_back();

      // Pop current node from node stack
      node_stack.pop_back();
      
      if (right_stack.size() != 0) {
         // Add right stacks to build stack
         build_stack.push_back(right_stack);

         // Create right node
         right_node = new KD_Node();
      
         // Link right nodes to current node
         node->right = right_node;

         // Set axis
         node->right->axis = (node->axis+1)%3;

         // Set max and min
         node->right->max = node->max;
         node->right->min = node->min;
         if (node->axis == 0) {
            node->right->min.x = median;
         }
         if (node->axis == 1) {
            node->right->min.y = median;
         }
         if (node->axis == 2) {
            node->right->min.z = median;
         }

         // Add right nodes to node stack
         node_stack.push_back(right_node);
      }
      if (left_stack.size() != 0) {
         // Add left stack to build stack
         build_stack.push_back(left_stack);

         // Create left node
         left_node = new KD_Node();
         
         // Link left nodes to current node
         node->left = left_node;

         // Set axis
         node->left->axis = (node->axis+1)%3;

         // Set max and min
         node->left->max = node->max;
         node->left->min = node->min;
         if (node->axis == 0) {
            node->left->max.x = median;
         }
         if (node->axis == 1) {
            node->left->max.y = median;
         }
         if (node->axis == 2) {
            node->left->max.z = median;
         }

         // Add left nodes to node stack
         node_stack.push_back(left_node);
      }
   }
}

void KD_Tree::Cull(Camera *cam, KD_Node *node) {
   Mesh *obj;
   Vector3D corners[8], max, min;
   Vector3D normal, v, sp, c1 ,c2, c3, cd;
   Vector3D cv;
   Triangle tri;
   int p, c;
   double dist, l1, l2, l3, radius;
   int numOutside;

   if (node != 0 && node->inactive == false) {
      // VFC
      max = node->max+position;
      min = node->min+position;
      corners[0].x = min.x;
      corners[0].y = max.y;
      corners[0].z = max.z;

      corners[1].x = max.x;
      corners[1].y = max.y;
      corners[1].z = max.z;

      corners[2].x = min.x;
      corners[2].y = min.y;
      corners[2].z = max.z;

      corners[3].x = max.x;
      corners[3].y = min.y;
      corners[3].z = max.z;

      corners[4].x = min.x;
      corners[4].y = max.y;
      corners[4].z = min.z;

      corners[5].x = max.x;
      corners[5].y = max.y;
      corners[5].z = min.z;

      corners[6].x = min.x;
      corners[6].y = min.y;
      corners[6].z = min.z;

      corners[7].x = max.x;
      corners[7].y = min.y;
      corners[7].z = min.z;

      for (p = 0; p < 6; p++) {
         numOutside = 0;
         for (c = 0; c < 8; c++) {
            dist = cam->planes[p].normal.Dot(corners[c]) + cam->planes[p].d;
            if (dist < 0) {
               // Outside
               numOutside++;
            }
         }
         if (numOutside == 8) {
            node->inactive = true;
            break;
         }
      }

      if (node->inactive == false) {
         obj = object;
         tri.c1 = *(obj->vertices[node->face->vertex[0]])+position;
         tri.c2 = *(obj->vertices[node->face->vertex[1]])+position;
         tri.c3 = *(obj->vertices[node->face->vertex[2]])+position;

         // Back face culling
         normal = (tri.c2-tri.c1).Cross(tri.c3-tri.c1);
         normal.Normalize();
         cv = (cam->lookat - cam->position);
         cv.Normalize();
         if (cv.Dot(normal) > 0.05) {
            node->skip = true;
         }
         else {
            v = cam->position-tri.c1;
            v.Normalize();
            c1 = tri.c1 - v*(tri.c1.Dot(cam->planes[4].normal) + cam->planes[4].d)/(v.Dot(cam->planes[4].normal));
            v = cam->position-tri.c2;
            v.Normalize();
            c2 = tri.c2 - v*(tri.c2.Dot(cam->planes[4].normal) + cam->planes[4].d)/(v.Dot(cam->planes[4].normal));
            v = cam->position-tri.c3;
            v.Normalize();
            c3 = tri.c3 - v*(tri.c3.Dot(cam->planes[4].normal) + cam->planes[4].d)/(v.Dot(cam->planes[4].normal));
            l1 = (c1 - c2).Length();
            l2 = (c1 - c3).Length();
            l3 = (c2 - c3).Length();
            if (l1 > l2) {
               if (l1 > l3) {
                  cd = (c1+c2)/2;
                  radius = (c1-cd).Length();
               }
               else {
                  cd = (c2+c3)/2;
                  radius = (c2-cd).Length();
               }
            }
            else {
               if (l2 > l3) {
                  cd = (c1+c3)/2;
                  radius = (c1-cd).Length();
               }
               else {
                  cd = (c2+c3)/2;
                  radius = (c2-cd).Length();
               }
            }

            node->sp = cd-cam->position;
            node->sp.Normalize();

            if (radius <= 1.0) {
               node->sp_radius = sqrt(1-(radius*radius))-0.001;
            }
            else {
               node->sp_radius = 0;
            }
         }

         // Continue traversal
         this->Cull(cam, node->left);
         this->Cull(cam, node->right);
      }
   }
}

void KD_Tree::Intersect(Ray *ray, KD_Node *node, bool &hit, bool optimize) {
   Mesh *obj;
   Triangle tri;
   double temp;
   Vector3D v;


   if (node != 0) {
      if (!node->skip || !optimize) {
         if (node->sp.Dot(ray->dir) > node->sp_radius || !optimize) {
            obj = object;

            // Triangle intersection
            tri.c1 = *(obj->vertices[node->face->vertex[0]])+position;
            tri.c2 = *(obj->vertices[node->face->vertex[1]])+position;
            tri.c3 = *(obj->vertices[node->face->vertex[2]])+position;
            temp = tri.Intersect(ray);
            if ((temp >= 0) && (temp < ray->dist)) {
               hit = true;
               ray->hit_tri = node->face;
               ray->dist = temp;
            }
         }
      }
      if (!node->inactive || !optimize) {
         // Ray-plane intersection
         if (!hit) {
            if (ray->pos[node->axis] <= node->face->max[node->axis]+position[node->axis]) { // Left
               this->Intersect(ray, node->left, hit, optimize);
               if (!hit && ray->dir[node->axis] >= -0.01) { // Right
                  this->Intersect(ray, node->right, hit, optimize);
               }
            }
            else { // Right
               this->Intersect(ray, node->right, hit, optimize);
               if (!hit && ray->dir[node->axis] <= 0.01) { // Left
                  this->Intersect(ray, node->left, hit, optimize);
               }
            }
         }
      }
   }
}

void KD_Tree::Delete(KD_Node *node) {
   if (node != 0) {
      this->Delete(node->left);
      this->Delete(node->right);
      delete node;
      node = 0;
   }
}
