#include "../include/tgaimage.hpp"

const TGAColor red = TGAColor(255, 0, 0, 255);

int main() {
    TGAImage image = TGAImage(100, 100, TGAImage::RGB);
    for (int i = 0; i < image.getWidth(); i++) {
        image.setColor(i, image.getHeight()>>1, red);
    }
    
    bool flag = image.write_tga_file("out.tga");
    return 0;   
}
