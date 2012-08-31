#include "Camera.h"


Camera::Camera(int width, int height) {
   this->width = width;
   this->height = height;
   aspect_ratio = (double)width/height;

   position.x = 0.0;
   position.y = 1.0;
   position.z = 1.0;
   lookat.x = 0.0;
   lookat.y = 0.0;
   lookat.z = 0.0;

   l = -aspect_ratio/2.0;
   r =  aspect_ratio/2.0;
   b = -0.5;
   t =  0.5;
   rlw = aspect_ratio/(width-1);
   tbh = 1.0/(height-1);
}

Camera::~Camera() {
}

void Camera::GenerateRays(std::list<Ray> *rays) {
   Ray ray;
   Vector3D u, v, w;
   Vector3D up(0.0, 1.0, 0.0);
   Vector3D s, t;
   int i, j;

   Vector3D corners[8];
   Vector3D v1, v2;
   
   w = lookat-position;
   w.Normalize();
   u = up.Cross(w);
   v = w.Cross(u);
   
   s.z = 1.0;
   for (j = 0; j < height; j++) {
      s.y = -(b+tbh*(j+0.5));
      for (i = 0; i < width; i++) {
         s.x = -(l+rlw*(i+0.5));
         t = s;
         t.Normalize();
         ray.dir = u*t.x + v*t.y + w*t.z;
         ray.pos = position + ray.dir;
         ray.dir.Normalize();
         ray.pixel = j*width + i;
         rays->push_back(ray);
         if (i == 0 && j == 0) {
            corners[0] = ray.pos;
            corners[4] = ray.pos + ray.dir*10000;
         }
         if (i == (width-1) && j == 0) {
            corners[1] = ray.pos;
            corners[5] = ray.pos + ray.dir*10000;
         }
         if (i == 0 && j == (height-1)) {
            corners[2] = ray.pos;
            corners[6] = ray.pos + ray.dir*10000;
         }
         if (i == (width-1) && j == (height-1)) {
            corners[3] = ray.pos;
            corners[7] = ray.pos + ray.dir*10000;
         }
      }
   }

   // Top
   v1 = corners[4] - corners[0];
   v2 = corners[1] - corners[0];
   planes[0].normal = v1.Cross(v2);
   planes[0].normal.Normalize();
   // Right
   v1 = corners[1] - corners[3];
   v2 = corners[7] - corners[3];
   planes[1].normal = v1.Cross(v2);
   planes[1].normal.Normalize();
   // Bottom
   v1 = corners[3] - corners[2];
   v2 = corners[6] - corners[2];
   planes[2].normal = v1.Cross(v2);
   planes[2].normal.Normalize();
   // Left
   v1 = corners[6] - corners[2];
   v2 = corners[0] - corners[2];
   planes[3].normal = v1.Cross(v2);
   planes[3].normal.Normalize();
   // Near
   v1 = corners[0] - corners[2];
   v2 = corners[3] - corners[2];
   planes[4].normal = v1.Cross(v2);
   planes[4].normal.Normalize();
   // Far
   v1 = corners[4] - corners[5];
   v2 = corners[7] - corners[5];
   planes[5].normal = v1.Cross(v2);
   planes[5].normal.Normalize();

   planes[0].d = -planes[0].normal.Dot(corners[0]);
   planes[1].d = -planes[1].normal.Dot(corners[7]);
   planes[2].d = -planes[2].normal.Dot(corners[7]);
   planes[3].d = -planes[3].normal.Dot(corners[0]);
   planes[4].d = -planes[4].normal.Dot(corners[0]);
   planes[5].d = -planes[5].normal.Dot(corners[7]);
}

