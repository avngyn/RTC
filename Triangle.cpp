#include "Triangle.h"


Triangle::Triangle() {
}

Triangle::~Triangle() {
}

double Triangle::Intersect(Ray *ray) {
   double a, b, c, d, e, f, g, h, i, j, k, l, M;
   double xa, xb, xc, xd, xe, ya, yb, yc, yd, ye, za, zb, zc, zd, ze;
   double eihf, gfdi, dheg, akjb, jcal, blkc;
   double beta, gamma, t;
   
   xa = c1.x;
   xb = c2.x;
   xc = c3.x;
   xd = (ray->dir).x;
   xe = (ray->pos).x;
   ya = c1.y;
   yb = c2.y;
   yc = c3.y;
   yd = (ray->dir).y;
   ye = (ray->pos).y;
   za = c1.z;
   zb = c2.z;
   zc = c3.z;
   zd = (ray->dir).z;
   ze = (ray->pos).z;
   a = xa-xb;
   b = ya-yb;
   c = za-zb;
   d = xa-xc;
   e = ya-yc;
   f = za-zc;
   g = xd;
   h = yd;
   i = zd;
   j = xa-xe;
   k = ya-ye;
   l = za-ze;
   eihf = e*i-h*f;
   gfdi = g*f-d*i;
   dheg = d*h-e*g;
   akjb = a*k-j*b;
   jcal = j*c-a*l;
   blkc = b*l-k*c;
   M = a*(eihf)+b*(gfdi)+c*(dheg);

   t = -(f*(akjb)+e*(jcal)+d*(blkc))/M;
   if (t < EPSILON) {
      return -1.0;
   }
   
   gamma = (i*(akjb)+h*(jcal)+g*(blkc))/M;
   if ((gamma < 0.0) || (gamma > 1.0)) {
      return -1.0;
   }
   
   beta = (j*(eihf)+k*(gfdi)+l*(dheg))/M;
   if ((beta < 0) || ( beta > (1-gamma))) {
      return -1.0;
   }
   return t;
}
