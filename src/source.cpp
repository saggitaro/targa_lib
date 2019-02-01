#include "../include/tgaimage.hpp"

const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255, 255);

int main() {
    TGAImage image = TGAImage(500, 500, TGAImage::RGB);
    
    for (int j = 0; j < image.getHeight(); j++) {
        if (j < image.getHeight()>>1) {
            image.setColor(image.getWidth()>>1, j, red);
        }
        else {
            image.setColor(image.getWidth()>>1, j, blue);
        }
    }
    image.write_tga_file("out.tga");
    image.FlipVertical();
    image.write_tga_file("out1.tga");
    
    
    
    return 0;   
}
