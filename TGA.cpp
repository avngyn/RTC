#include <iostream>
#include <fstream>

#include "TGA.h"


TGA::TGA(short width, short height) {
   int size, i;

   IDLength = 0;
   ColorMapType = 0;
   ImageType = 2;
   ColorMapOffset = 0;
   ColorMapLength = 0;
   ColorMapEntrySize = 0;
   ImageXOrigin = 0;
   ImageYOrigin = 0;
   ImageWidth = width;
   ImageHeight = height;
   PixelDepth = 24;
   ImageDescriptor = 1<<5;

   size = width * height;
   pixels = new Pixel[size];
   for (i = 0; i < size; i++) {
      pixels[i].r = 0;
      pixels[i].g = 0;
      pixels[i].b = 0;
   }
}

TGA::~TGA() {
   if (pixels != 0) {
      delete[] pixels;
   }
}

bool TGA::Write(const char *name) {
   std::ofstream file;
   std::string filename;
   int size, i;
   
   // Create TGA file
   filename = name;
   filename += ".tga";
   file.open(filename, std::ios::binary);
   if (file.good() == false) {
      std::cout << "Error: Unable to open " << filename.c_str() << "." << std::endl;
      return false;
   }
   
   // Write header
   file << (char)IDLength <<
           (char)ColorMapType <<
           (char)ImageType <<
           (char)(ColorMapOffset&0x00FF) <<
           (char)((ColorMapOffset&0xFF00)/256) <<
           (char)(ColorMapLength&0x00FF) <<
           (char)((ColorMapLength&0xFF00)/256) <<
           (char)ColorMapEntrySize <<
           (char)(ImageXOrigin&0x00FF) <<
           (char)((ImageXOrigin&0xFF00)/256) <<
           (char)(ImageYOrigin&0x00FF) <<
           (char)((ImageYOrigin&0xFF00)/256) <<
           (char)(ImageWidth&0x00FF) <<
           (char)((ImageWidth&0xFF00)/256) <<
           (char)(ImageHeight&0x00FF) <<
           (char)((ImageHeight&0xFF00)/256) <<
           (char)PixelDepth <<
           (char)ImageDescriptor;

   // Write data
   size = ImageWidth * ImageHeight;
   for (i = 0; i < size; i++) {
      file << (char)(pixels[i]).b <<
              (char)(pixels[i]).g <<
              (char)(pixels[i]).r;
   }

   file.close();

   return true;
}
