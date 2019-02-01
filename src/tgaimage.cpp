#include "../include/tgaimage.hpp"

TGAImage::TGAImage() : data(NULL), width(0), height(0), bytespp(0) {}

TGAImage::TGAImage(int w, int h, int bpp) : data(NULL), width(w), height(h), bytespp(bpp) {
    unsigned long nbytes = width * height * bytespp;
    data = new unsigned char[nbytes];
    memset(data, 0, nbytes);
}

TGAImage::TGAImage(const TGAImage &img) : data(NULL), width(img.width), height(img.height), bytespp(img.bytespp) {
    unsigned long nbytes = width * height * bytespp;
    data = new unsigned char[nbytes];
    memcpy(data, img.data, nbytes);
}

TGAImage::~TGAImage() {
    if (data)
        delete [] data;
}

TGAImage & TGAImage::operator =(const TGAImage &img) {
    if (this != &img) {
        if (data)
            delete [] data;
        width = img.width;
        height = img.height;
        bytespp = img.bytespp;
        unsigned long bytes = width * height * bytespp;
        data = new unsigned char[bytes];
        memcpy(data, img.data, bytes);
    }
    return *this;
}

//TODO need rle implementaion
bool TGAImage::write_tga_file(const char *filename) {
    unsigned char developer_area[4] = {0, 0, 0, 0};
    unsigned char extension_area[4] = {0, 0, 0, 0};
    unsigned char footer[18] = {'T', 'R', 'U', 'E', 'V', 'I', 'S', 'I', 'O', 'N', '-', 'X', 'F', 'I', 'L', 'E', '.', '\0'};

    std::ofstream out;
    out.open(filename, std::ios::binary);
    if (!out.is_open()) {
        std::cerr << "can't open file " << filename << std::endl;
        out.close();
        return false;
    }
    TGAheader header;
    memset((void *)&header, 0, sizeof(header));
    header.bitsperpixel = bytespp<<3;
    header.width = width;
    header.height = height;
    header.imagetype = (bytespp==GRAYSCALE ? 3 : 2);
    header.imagedescr = 0x20;
    out.write((char *)&header, sizeof(header));
    if (!out.good()) {
        out.close();
        std::cerr << "can't dump tga file" << std::endl;
        return false;
    }

    out.write((char *)data, width * height * bytespp);
    if (!out.good()) {
        std::cerr << "can't unload raw data" << std::endl;
        out.close();
        return false;
    }

    out.write((char *)developer_area, sizeof(developer_area));
    if (!out.good()) {
        std::cerr << "can't dump developer area" << std::endl;
        out.close();
        return false;
    }

    out.write((char *)extension_area, sizeof(extension_area));
    if (!out.good()) {
        std::cerr << "can't dump extension area" << std::endl;
        out.close();
        return false;
    }

    out.write((char *)footer, sizeof(footer));
    if (!out.good()) {
        std::cerr << "can't dump footer" << std::endl;
        out.close();
        return false;
    }

    out.close();
    return true;
}

bool TGAImage::PrintToConsole() {
    if (!data)
        return false;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            TGAColor c = getColor(i, j);
            std::cout << "(" << (int)c.rgba[2] << ":" << (int)c.rgba[1] << ":" << (int)c.rgba[0] << ":" << (int)c.rgba[3] << ") ";
        }
        std::cout << std::endl;
    }
}

bool TGAImage::FlipHorizontal() {
    if (!data)
        return false;

    int pivot = width >> 1;
    for (int i = 0; i < pivot; i++) {
        for (int j = 0; j < height; j++) {
            TGAColor c1 = getColor(i, j);
            TGAColor c2 = getColor(width - 1 - i, j);
            setColor(i, j, c2);
            setColor(width - 1 - i, j, c1);
        }
    }
    return true;
}

bool TGAImage::FlipVertical() {
    if (!data)
        return false;
    
    int pivot = height >> 1;
    unsigned long bytespl = width * bytespp;
    unsigned char* buff = new unsigned char[bytespl];
    for (int j = 0; j < pivot; j++) {
        unsigned long line1pos = j * bytespl;
        unsigned long line2pos = (height - 1 - j) * bytespl;
        memmove((void *)buff, (void *)(data + line1pos), bytespl);
        memmove((void *)(data + line1pos), (void *)(data + line2pos), bytespl);
        memmove((void *)(data + line2pos), (void *)buff, bytespl);
    }
    delete [] buff;
    return true;
}

TGAColor TGAImage::getColor(int x, int y) {
    if (!data || x < 0 || y < 0 || x >= width || y >= height)
        return TGAColor();
    return TGAColor(data + (x + y * width) * bytespp, bytespp);
}

bool TGAImage::setColor(int x, int y, TGAColor &color) {
    if (!data || x < 0 || y < 0 || x >= width || y >= height)
        return false;
    memcpy(data + (x + y*width)*bytespp, color.rgba, bytespp);
    return true;
}

bool TGAImage::setColor(int x, int y, const TGAColor &color) {
    if (!data || x < 0 || y < 0 || x >= width || y >= height)
        return false;
    memcpy(data + (x + y*width)*bytespp, color.rgba, bytespp);
    return true;
}

int TGAImage::getWidth() {
    return width;
}

int TGAImage::getHeight() {
    return height;
}

int TGAImage::getBytespp() {
    return bytespp;
}

unsigned char *TGAImage::buffer() {
    return data;
}

void TGAImage::clear() {
    memset((void *)data, 0, width * height * bytespp);
}