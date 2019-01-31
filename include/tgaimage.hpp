#ifndef __TGA_IMAGE__
#define __TGA_IMAGE__

#include <cstring>
#include <iostream>
#include <fstream>

#pragma pack(push, 1)
struct TGAheader {
    char idlength;
    char colormaptype;
    char imagetype;
    short colormaporigin;
    short colormaplength;
    char colormapdepth;
    short x_origin;
    short y_origin;
    short width;
    short height;
    char bitsperpixel;
    char imagedescr;
};
#pragma pack(pop)

struct TGAColor {
    unsigned char rgba[4];
    unsigned char bytespp;

    TGAColor() : rgba(), bytespp(1) {
        for (int i = 0; i < 4; i++)
            rgba[i] = 0;
    }

    TGAColor(unsigned char R, unsigned char G, unsigned char B, unsigned char A = 255) : rgba(), bytespp(4) {
        rgba[0] = B;
        rgba[1] = G;
        rgba[2] = R;
        rgba[3] = A;
    }

    TGAColor(const unsigned char *color, unsigned char bpp) : rgba(), bytespp(bpp) {
        for (int i = 0; i < (int)bpp; i++)
            rgba[i] = color[i];
        
        for (int i = bpp; i < 4; i++)
            rgba[i] = 0;
    }

    unsigned char& operator[](const int i) {
        return rgba[i];
    }

    TGAColor operator *(float intensity) const {
        TGAColor res = *this;
        intensity = (intensity > 1.f ? 1.f : (intensity < 0 ? 0.f : intensity));
        for (int i = 0; i < 4; i++)
            res.rgba[i] = rgba[i] * intensity;
        
        return res;
    }
};

class TGAImage {
protected:
    unsigned char* data;
    int width;
    int height;
    int bytespp;

public:
    enum Format {
        GRAYSCALE=1, RGB=3, RGBA=4
    };

    TGAImage();
    TGAImage(int w, int h, int bpp);
    TGAImage(const TGAImage &img);
    ~TGAImage();
    TGAImage & operator =(const TGAImage &img);

    bool write_tga_file(const char *filename);

    bool PrintToConsole();

    TGAColor getColor(int x, int y);
    bool setColor(int x, int y, TGAColor &color);
    bool setColor(int x, int y, const TGAColor &color);
    
    int getWidth();
    int getHeight();
    int getBytespp();
};

#endif