#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "OBJ.h"


OBJ::OBJ() {
}

OBJ::~OBJ() {
}

bool OBJ::Open(const char *filename) {
   std::ifstream file;
   std::stringstream buffer, temp;
   std::string token;
   Vector3D *v;
   Face *f;
   bool has_texture;
   char c;
   int s, t, num_divs;
   int i;

   // Open file
   file.open(filename);
   if (!file.good()) {
      std::cout << "Error: Unable to open " << filename << std::endl;
   }

   // Read file to string stream
   buffer << file.rdbuf();

   // Close file
   file.close();

   // Parse file
	while (buffer.good()) {
		buffer >> token;

		// Handle vertices
		if (token.compare("v") == 0) {
         v = new Vector3D();
			buffer >> v->x >> v->y>> v->z;
			vertices.push_back(v);
		}

		// Handle texture coordinates
		else if (token.compare("vt") == 0) {
         v = new Vector3D();
			buffer >> v->x >> v->y;
			texcoords.push_back(v);
		}

		// Handle normals
		else if (token.compare("vn") == 0) {
         v = new Vector3D();
			buffer >> v->x >> v->y >> v->z;
			normals.push_back(v);
		}

		// Handle faces
		else if (token.compare("f") == 0) {
         f = new Face();
			for (i = 0; i < 3; i++) {
            // Count number of divisions
            temp.clear();
			   buffer >> token;
            temp << token;
            num_divs = -1;
            has_texture = true;
            for (s = 0, t = 0; t != std::string::npos; s = t) {
               t = token.find('/', s+1);
               if ((s != 0) && (t == s+1))  {
                  has_texture = false;
               }
               num_divs++;
            }

            if (num_divs == 0) {
               temp >> f->vertex[i];
            }
            else if (num_divs == 1) {
               temp >> f->vertex[i] >> c >> f->texcoord[i];
               f->texcoord[i] -= 1;
            }
            else if (num_divs == 2) {
               if (has_texture) {
                  temp >> f->vertex[i] >> c >> f->texcoord[i] >> c >> f->normal[i];
                  f->texcoord[i] -= 1;
               }
               else {
                  temp >> f->vertex[i] >> c >> c >> f->normal[i];
               }
               f->normal[i] -= 1;
            }
            f->vertex[i] -= 1;
         }

			faces.push_back(f);
		}

		// Handle comments
		else if (token.compare("#") == 0) {
			// Do nothing
		}

		// Handle unknown
		else {
			// Do nothing
		}
	}

   return true;
}
