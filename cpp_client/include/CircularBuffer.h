#pragma once
#include <vector>

#ifdef _WIN32
  #include <windows.h>     // Ensure APIENTRY/WINGDIAPI defined before GL on Windows
#endif
#include <GL/freeglut.h>    // Pulls in GL headers in the right order

// Owns an OpenGL texture used to display the image.
// Resizable to any (width,height) per-archive.
class CircularBuffer {
public:
    int width  = 1;
    int height = 1;
    GLuint texture = 0;

    // Optional CPU staging buffer (not required for GL).
    std::vector<uint32_t> buffer;

    CircularBuffer() = default;

    ~CircularBuffer() {
        if (texture != 0) {
            glDeleteTextures(1, &texture);
            texture = 0;
        }
    }

    // Create (or recreate) the GPU texture to match a new size.
    // IMPORTANT: Call this ONLY after an OpenGL context exists
    // (i.e., after glutCreateWindow or equivalent).
    void reset(int w, int h) {
        width  = (w > 0) ? w : 1;
        height = (h > 0) ? h : 1;

        // Keep CPU buffer the same size as the image (optional)
        buffer.assign(static_cast<size_t>(width) * height, 0);

        // Destroy previous texture if present
        if (texture != 0) {
            glDeleteTextures(1, &texture);
            texture = 0;
        }

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        // Allocate GPU storage only. Pass nullptr to avoid reading from CPU.
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
