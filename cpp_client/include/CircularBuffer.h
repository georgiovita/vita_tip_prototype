#pragma once
#include <vector>
#include <GL/gl.h>



class CircularBuffer {
public:
    int width  = 1;
    int height = 1;
    GLuint texture = 0;



    std::vector<uint32_t> buffer;

    CircularBuffer() = default;

    ~CircularBuffer() {
        if (texture != 0) {
            glDeleteTextures(1, &texture);
            texture = 0;
        }
    }




    void reset(int w, int h) {
        width  = (w > 0) ? w : 1;
        height = (h > 0) ? h : 1;


        buffer.assign(static_cast<size_t>(width) * height, 0);


        if (texture != 0) {
            glDeleteTextures(1, &texture);
            texture = 0;
        }

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);


        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            width,
            height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            nullptr
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};
