#include <vector>
#include "CircularBuffer.h"

class CircularBuffer {
public:
    int width = 1146;
    int height = 704;
    std::vector<uint32_t> buffer;

    CircularBuffer() {
        buffer.resize(width * height, 0);
    }
};

void insertRawline(std::vector<uint16_t> &i_raw_line, int column) {
    for (int y = 0; y < height; y++) {
        uint8_t pixel = i_raw_line[y] >> 8;
        uint32_t rgba = (255 << 24) | (pixel << 16) | (pixel << 8) | pixel;
        buffer[y * width + column] = rgba;
    }
}
};
