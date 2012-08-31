#ifndef OBJ_H
#define OBJ_H


#include "Mesh.h"


class OBJ : public Mesh {
   public:
      OBJ();
      ~OBJ();

      bool Open(const char *filename);
};


#endif // OBJ_H
