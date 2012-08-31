#ifndef MEDIAN_H
#define MEDIAN_H


#include <vector>

#include "Mesh.h"


void Median(std::vector<Face *> &faces, int axis, unsigned int &k) {
   Face *f;
   int i, j, l, m;
   double x;

   l = 0;
   m = faces.size()-1;
   while (l < m) {
      x = faces[k]->max[axis];
      i = l;
      j = m;
      do {
         while (faces[i]->max[axis] < x) {
            i++;
         }
         while (x < faces[j]->max[axis]) {
            j--;
         }
         if (i <= j) {
            f = faces[i];
            faces[i] = faces[j];
            faces[j] = f;
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
}


#endif // MEDIAN_H
