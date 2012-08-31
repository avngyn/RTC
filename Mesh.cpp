#include <iostream>

#include "Mesh.h"


Mesh::Mesh() {
   vertices.clear();
   faces.clear();
   normals.clear();
   texcoords.clear();
   position.x = 0;
   position.y = 0;
   position.z = 0;
}

Mesh::~Mesh() {
   unsigned int num, i;
   num = vertices.size();
   for (i = 0; i < num; i++) {
      if (vertices[i]) {
         delete vertices[i];
         vertices[i] = 0;
      }
   }
   num = faces.size();
   for (i = 0; i < num; i++) {
      if (faces[i]) {
         delete faces[i];
         faces[i] = 0;
      }
   }
   num = normals.size();
   for (i = 0; i < num; i++) {
      if (normals[i]) {
         delete normals[i];
         normals[i] = 0;
      }
   }
   num = texcoords.size();
   for (i = 0; i < num; i++) {
      if (texcoords[i]) {
         delete texcoords[i];
         texcoords[i] = 0;
      }
   }
   num = centers.size();
   for (i = 0; i < num; i++) {
      if (centers[i]) {
         delete centers[i];
         centers[i] = 0;
      }
   }
}

void Mesh::CalculateMaxMin() {
   Face *f;
   Vector3D mesh_max, mesh_min;
   unsigned int num_faces;
   unsigned int i, v;

   num_faces = faces.size();
   mesh_max = *(vertices[faces[0]->vertex[0]]);
   mesh_min = *(vertices[faces[0]->vertex[0]]);
   for (i = 0; i < num_faces; i++) {
      f = faces[i];
      f->max = *(vertices[f->vertex[0]]);
      f->min = *(vertices[f->vertex[0]]);

      // Face max min
      for (v = 1; v <= 2; v++) {
         if ((vertices[f->vertex[v]])->x > f->max.x) {
            f->max.x = (vertices[f->vertex[v]])->x;
         }
         if ((vertices[f->vertex[v]])->x < f->min.x) {
            f->min.x = (vertices[f->vertex[v]])->x;
         }
         if ((vertices[f->vertex[v]])->y > f->max.y) {
            f->max.y = (vertices[f->vertex[v]])->y;
         }
         if ((vertices[f->vertex[v]])->y < f->min.y) {
            f->min.y = (vertices[f->vertex[v]])->y;
         }
         if ((vertices[f->vertex[v]])->z > f->max.z) {
            f->max.z = (vertices[f->vertex[v]])->z;
         }
         if ((vertices[f->vertex[v]])->z < f->min.z) {
            f->min.z = (vertices[f->vertex[v]])->z;
         }
      }

      // Mesh max min
      if (f->max.x > mesh_max.x) {
         mesh_max.x = f->max.x;
      }
      if (f->min.x < mesh_min.x) {
         mesh_min.x = f->min.x;
      }
      if (f->max.y > mesh_max.y) {
         mesh_max.y = f->max.y;
      }
      if (f->min.y < mesh_min.y) {
         mesh_min.y = f->min.y;
      }
      if (f->max.z > mesh_max.z) {
         mesh_max.z = f->max.z;
      }
      if (f->min.z < mesh_min.z) {
         mesh_min.z = f->min.z;
      }
   }
   center = (mesh_max + mesh_min)/2;
   max = mesh_max - center;
   min = mesh_min - center;
   radius = max.x;
   if (max.y > radius) {
      radius = max.y;
   }
   if (max.z > radius) {
      radius = max.z;
   }
}

void Mesh::CalculateNormals() {
   Face *f;
   Vector3D a, b, c, *d;
   unsigned int num;
   unsigned int i;
   
   // Delete existing normals
   num = normals.size();
   for (i = 0; i < num; i++) {
      if (normals[i]) {
         delete normals[i];
         normals[i] = 0;
      }
   }
   normals.clear();

   num = vertices.size();
   for (i = 0; i < num; i++) {
      d = new Vector3D();
      normals.push_back(d);
   }

   num = faces.size();
   for (i = 0; i < num; i++) {
      f = faces[i];
      a = *vertices[f->vertex[2]]-*vertices[f->vertex[0]];
      b = *vertices[f->vertex[1]]-*vertices[f->vertex[0]];
      c = b.Cross(a);

      *normals[f->vertex[0]] += c;
      *normals[f->vertex[1]] += c;
      *normals[f->vertex[2]] += c;

      f->normal[0] = f->vertex[0];
      f->normal[1] = f->vertex[1];
      f->normal[2] = f->vertex[2];
   }

   num = normals.size();
   for (i = 0; i < num; i++) {
      normals[i]->Normalize();
   }
}
