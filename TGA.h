#ifndef TGA_H
#define TGA_H


struct Pixel {
   unsigned char r, g, b;
};


class TGA {
   public:
      TGA(short width, short height);
      ~TGA();
      
      bool Write(const char *name);
   public:
      // Header
      char IDLength;           // 0-255
      char ColorMapType;       // 0 = none
                               // 1 = has color map
      char ImageType;          // 0 = none
                               // 1 = uncompressed color-mapped image
                               // 2 = uncompressed true-color image
                               // 3 = uncompressed black-and-white image
                               // 9 = run-length encoded color map image
                               // 10 = run-length encoded true-color image
                               // 11 = run-length encoded black-and-white image
      short ColorMapOffset;    // Offset to color map table
      short ColorMapLength;    // Number of entries
      char ColorMapEntrySize;  // Number of bits per pixel for color map
      short ImageXOrigin;      // Absolute coordinate of origin
      short ImageYOrigin;      // Absolute coordinate of origin
      short ImageWidth;        // Width in pixels
      short ImageHeight;       // Height in pixels
      char PixelDepth;         // Number of bits per pixel for image
      char ImageDescriptor;    // Bit 3-0: alpha channel depth
                               //    Targa 16 = 0
                               //    Targa 24 = 0
                               //    Targa 32 = 8
                               // Bit 4: reserved, must be 0
                               // Bit 5: screen origin
                               //    0 = origin in lower left-hand corner
                               //    1 = origin in upper left-hand corner
                               // Bit 7-6: data storage interleaving flag
                               //    00 = non-interleaved
                               //    01 = two-way (even/odd) interleaving
                               //    10 = four-way interleaving
                               //    11 = reserved

      // Data
      Pixel *pixels;
};


#endif // TGA_H
