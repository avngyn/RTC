#include "RayTracer.h"


RayTracer::RayTracer() : camera(SCREEN_WIDTH, SCREEN_HEIGHT), frame(SCREEN_WIDTH, SCREEN_HEIGHT) {
   Light l;

   // Set camera
   camera.position.x = -2.75;
   camera.position.y = 2.0;
   camera.position.z = 3.0;
   camera.lookat.x = -1.25;
   camera.lookat.y = 1.5;
   camera.lookat.z = -2.0;

   // Set lights
   l.color.x = 1.0;
   l.color.y = 1.0;
   l.color.z = 1.0;
   l.pos.x = -20.0;
   l.pos.y = 20.0;
   l.pos.z = 20.0;
   lights.push_back(l);
}

RayTracer::~RayTracer() {
   std::vector<Mesh *>::iterator it;
   for (it = objects.begin(); it != objects.end(); it++) {
      if (*it) {
         delete *it;
      }
   }
}

bool RayTracer::Initialize() {
   bool result;

   result = this->Load("bunny.obj");
   objects[0]->position.x = 0.0;
   objects[0]->position.y = 0.0;
   objects[0]->position.z = 0.0;
   kdtrees[0]->position = objects[0]->position;

   objects[0]->pigment.x = 0.8;
   objects[0]->pigment.y = 0.1;
   objects[0]->pigment.z = 0.1;

   objects[0]->ambient = 0.5;
   objects[0]->diffuse = 0.9;
   objects[0]->specular = 0.1;
   objects[0]->reflection = 0.0;
   objects[0]->refraction = 0.0;
   objects[0]->roughness = 0.05;
   objects[0]->ior = 0.0;

   result = this->Load("wall.obj");
   objects[1]->position.x = 0.0;
   objects[1]->position.y = 0.0;
   objects[1]->position.z = 0.0;
   kdtrees[1]->position = objects[1]->position;

   objects[1]->pigment.x = 0.1;
   objects[1]->pigment.y = 0.2;
   objects[1]->pigment.z = 0.7;

   objects[1]->ambient = 0.5;
   objects[1]->diffuse = 0.9;
   objects[1]->specular = 0.1;
   objects[1]->reflection = 0.9;
   objects[1]->refraction = 0.0;
   objects[1]->roughness = 0.05;
   objects[1]->ior = 0.0;
   return result;
}

bool RayTracer::Load(const char *filename) {
   OBJ *obj;
   KD_Tree *kdtree;
   bool result;

   obj = new OBJ();

   // Load model
   result = obj->Open(filename);
   objects.push_back(obj);

   obj->CalculateNormals();
   obj->CalculateMaxMin();

   // Build K-D tree
   kdtree = new KD_Tree();
   kdtree->object = obj;
   kdtree->Build();
   kdtrees.push_back(kdtree);

   return result;
}

void RayTracer::Render() {
   Ray ray;
   bool hit;
   Vector3D color;
   Pixel pixel;
   int i;
   
   // Generate initial cast rays
   this->Cast();

   for (i = 0; i < kdtrees.size(); i++) {
      kdtrees[i]->Cull(&camera, kdtrees[i]->root);
   }

   // Test intersection for each ray
   while (!cast_rays.empty()) {
      // Get ray from list
      ray = cast_rays.front();
      cast_rays.pop_front();
      
      // Test intersection
      hit = this->Intersect(&ray, true);
      if (hit) {
         light_rays.push_back(ray);
      }
      else {
         frame.pixels[ray.pixel].r = 0;
         frame.pixels[ray.pixel].g = 0;
         frame.pixels[ray.pixel].b = 0;
      }
   }

   // Calculate lighting for each light ray
   while (!light_rays.empty()) {
      // Get ray from list
      ray = light_rays.front();
      light_rays.pop_front();

      // Calculate lighting
      this->Lighting(&ray, color);

      // Scale light contributions
      color *= 255;

      // Bound color values
      if (color.x < 0.0) {
         color.x = 0;
      }
      else if (color.x > 255.0) {
         color.x = 255;
      }
      if (color.y < 0.0) {
         color.y = 0;
      }
      else if (color.y > 255.0) {
         color.y = 255;
      }
      if (color.z < 0.0) {
         color.z = 0;
      }
      else if (color.z > 255.0) {
         color.z = 255;
      }

      // Set pixel color
      pixel.r = (unsigned char)color.x;
      pixel.g = (unsigned char)color.y;
      pixel.b = (unsigned char)color.z;
      frame.pixels[ray.pixel] = pixel;
   }

   // Output frame to file
   this->Output();
}

void RayTracer::Clean() {
   int i;
   for (i = 0; i < kdtrees.size(); i++) {
      kdtrees[i]->Delete(kdtrees[i]->root);
   }
}

void RayTracer::Cast() {
   camera.GenerateRays(&cast_rays);
}

bool RayTracer::Intersect(Ray *ray, bool optimize) {
   bool hit, h;
   int i;

   hit = false;
   ray->dist = FAR_PLANE;

   // Test ray against list of objects
   for (i = 0; i < kdtrees.size(); i++) {
      h = false;
      kdtrees[i]->Intersect(ray, kdtrees[i]->root, h, optimize);
      if (h) {
         hit = true;
         ray->hit_obj = i;
      }
   }

   return hit;
}

void RayTracer::Lighting(Ray *ray, Vector3D &color) {
   Mesh *obj;
   Face *face;
   Light light;
   Ray light_ray, reflect_ray;
   Vector3D normal, reflect_color;
   Vector3D v, h;
   Vector3D n1, n2, n3;
   double dnl, svr;
   double dn;
   unsigned int num_lights;
   unsigned int i;
   int count;
   bool hit;

   num_lights = lights.size();

   // Get object
   obj = objects[ray->hit_obj];

   // Get object's face
   face = ray->hit_tri;

   // Get intersection point
   light_ray.pos = ray->pos + ray->dir * ray->dist;

   // Get object's normal
   n1 = *(obj->vertices[face->vertex[0]])+obj->position-light_ray.pos;
   n2 = *(obj->vertices[face->vertex[1]])+obj->position-light_ray.pos;
   n3 = *(obj->vertices[face->vertex[2]])+obj->position-light_ray.pos;
   normal = *(obj->normals[face->normal[0]])/n1.Length() + *(obj->normals[face->normal[1]])/n2.Length() + *(obj->normals[face->normal[2]])/n3.Length();
   normal.Normalize();

   // Calculate ambient color
   color = obj->pigment * obj->ambient;

   for (i = 0; i < num_lights; i++) {
      // Get light source
      light = lights[i];

      // Calculate light ray
      light_ray.dir = light.pos - light_ray.pos;
      light_ray.dist = light_ray.dir.Length();
      light_ray.dir.Normalize();

      // Intersection
      hit = this->Intersect(&light_ray, false);
      if (hit) { // No direct path to light (ie, in shadow)
      }
      else {
         // Calculate diffuse component
         dnl = normal.Dot(light_ray.dir);
         if (dnl < 0.0) {
            dnl = 0.0;
         }
         dnl *= obj->diffuse;

         // Calculate specular component
         v = light_ray.pos-camera.position;
         v.Normalize();
         h = v+light_ray.dir;
         h.Normalize();
         svr = h.Dot(normal);
         if (svr < 0.0) {
            svr = 0.0;
         }
         svr = obj->specular*pow(svr, 1.0/obj->roughness);

         // Calculate diffuse and specular color
         color.x += (dnl*obj->pigment.x*light.color.x + svr*light.color.x)/num_lights;
         color.y += (dnl*obj->pigment.y*light.color.y + svr*light.color.y)/num_lights;
         color.z += (dnl*obj->pigment.z*light.color.z + svr*light.color.z)/num_lights;
      }
   }

   if (obj->refraction == 1.0) {
      // Calculate refraction
   }
   else if (obj->reflection != 0.0) {
      // Create reflection ray
      reflect_ray.dir = ray->dir - normal*2.0*ray->dir.Dot(normal);
      reflect_ray.dir.Normalize();
      reflect_ray.pos = light_ray.pos;
      hit = this->Intersect(&reflect_ray, false);
      if (hit) {
         this->Lighting(&reflect_ray, reflect_color);
         color = color + reflect_color*obj->reflection;
      }
      else {
      }
   }
}

void RayTracer::Output() {
   frame.Write("output");
}
