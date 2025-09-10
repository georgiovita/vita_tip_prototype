#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>

#include "ArchiveClient.h"
#include "CircularBuffer.h"


#include "third_party/json.hpp"
using json = nlohmann::json;

CircularBuffer gBuffer;


enum EnergyMode {
    ENERGY_1_ONLY   = 0,
    ENERGY_2_ONLY   = 1,
    ENERGY_COMBINED = 2
};

EnergyMode current_energy_mode = ENERGY_COMBINED;


std::vector<uint16_t> g_archive_samples;


int g_img_width      = 0;
int g_img_height     = 0;
int g_num_energies   = 2;


bool g_flip_x = false;
bool g_flip_y = true;


static int as_int(const json& j, const char* key, int def) {
    if (!j.contains(key)) return def;
    const auto& v = j.at(key);
    if (v.is_number_integer()) return v.get<int>();
    if (v.is_number_float())   return static_cast<int>(std::lround(v.get<double>()));
    if (v.is_string()) {
        try { return std::stoi(v.get<std::string>()); } catch (...) { return def; }
    }
    return def;
}

struct ArchiveMeta {
    int width = 0;
    int height = 0;
    int numEnergies = 2;
};

static ArchiveMeta loadMetaFromJson(const std::string& jsonPath) {
    ArchiveMeta meta;
    std::ifstream f(jsonPath);
    if (!f.is_open()) {
        std::cerr << "Failed to open JSON: " << jsonPath << "\n";
        return meta;
    }
    json j;
    try {
        f >> j;
    } catch (const std::exception& e) {
        std::cerr << "JSON parse error in " << jsonPath << ": " << e.what() << "\n";
        return meta;
    }

    meta.width       = as_int(j, "Width", 0);
    meta.height      = as_int(j, "Height", 0);
    meta.numEnergies = as_int(j, "NumEnergies", 2);
    return meta;
}




static void processAndUploadImage() {
    if (g_archive_samples.empty()) return;

    const int W = g_img_width;
    const int H = g_img_height;
    const int E = g_num_energies;

    const size_t expected = static_cast<size_t>(W) * H * E;
    if (g_archive_samples.size() < expected) {
        std::cerr << "Archive too small: have " << g_archive_samples.size()
                  << " samples, expected " << expected << "\n";
        return;
    }

    std::vector<uint8_t> rgba(static_cast<size_t>(W) * H * 4, 0);

    for (int col = 0; col < W; ++col) {
        const int column_block = col * (E * H);

        for (int row = 0; row < H; ++row) {

            const int idx_e1 = column_block + row;
            uint16_t e1 = g_archive_samples[idx_e1];

            uint16_t e2 = e1;
            if (E >= 2) {
                const int idx_e2 = column_block + H + row;
                e2 = g_archive_samples[idx_e2];
            }

            uint16_t combined = 0;
            if (E <= 2) {
                combined = static_cast<uint16_t>((static_cast<uint32_t>(e1) + e2) / 2);
            } else {
                uint32_t sum = 0;
                for (int k = 0; k < E; ++k) {
                    const int idx_k = column_block + k * H + row;
                    sum += g_archive_samples[idx_k];
                }
                combined = static_cast<uint16_t>(sum / static_cast<uint32_t>(E));
            }

            uint16_t v16 = 0;
            switch (current_energy_mode) {
                case ENERGY_1_ONLY:   v16 = e1;       break;
                case ENERGY_2_ONLY:   v16 = e2;       break;
                case ENERGY_COMBINED: v16 = combined; break;
            }

            const uint8_t v8 = static_cast<uint8_t>(v16 >> 8); // 16→8 mapping

            const int dr = g_flip_y ? (H - 1 - row) : row;
            const int dc = g_flip_x ? (W - 1 - col) : col;

            const size_t o = (static_cast<size_t>(dr) * W + dc) * 4;
            rgba[o + 0] = v8;
            rgba[o + 1] = v8;
            rgba[o + 2] = v8;
            rgba[o + 3] = 255;
        }
    }

    glBindTexture(GL_TEXTURE_2D, gBuffer.texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, W, H, GL_RGBA, GL_UNSIGNED_BYTE, rgba.data());
    glBindTexture(GL_TEXTURE_2D, 0);

    glutPostRedisplay();
}

static void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, gBuffer.texture);

    glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex2f(0,0);
        glTexCoord2f(1,0); glVertex2f(static_cast<GLfloat>(gBuffer.width), 0);
        glTexCoord2f(1,1); glVertex2f(static_cast<GLfloat>(gBuffer.width), static_cast<GLfloat>(gBuffer.height));
        glTexCoord2f(0,1); glVertex2f(0, static_cast<GLfloat>(gBuffer.height));
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    glutSwapBuffers();
}

static void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glOrtho(0, static_cast<GLfloat>(gBuffer.width),
            0, static_cast<GLfloat>(gBuffer.height),
            -1, 1);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
}

static void keyboard(unsigned char key, int, int) {
    switch (key) {
        case '1': current_energy_mode = ENERGY_1_ONLY;   processAndUploadImage(); break;
        case '2': current_energy_mode = ENERGY_2_ONLY;   processAndUploadImage(); break;
        case '3': current_energy_mode = ENERGY_COMBINED; processAndUploadImage(); break;
        case 'h': case 'H': g_flip_x = !g_flip_x; processAndUploadImage(); break;
        case 'v': case 'V': g_flip_y = !g_flip_y; processAndUploadImage(); break;
        case 'q': case 'Q': case 27: std::exit(0); break;
        default:
            std::cout << "Controls: 1=E1, 2=E2, 3=Avg, H=flipX, V=flipY, Q=Quit\n";
            break;
    }
}


int main(int argc, char** argv) {

    const std::string basePath = "C:/Users/GeorgioHayek/Desktop/test/scanner_project/sample_archives/";


    std::string number;
    std::cout << "Enter archive number (e.g. 000355): ";
    std::cin >> number;

    const std::string vitaPath = basePath + number + ".vita";
    const std::string jsonPath = basePath + number + ".json";


    const ArchiveMeta meta = loadMetaFromJson(jsonPath);
    if (meta.width <= 0 || meta.height <= 0) {
        std::cerr << "Invalid metadata (Width/Height) in: " << jsonPath << "\n";
        return 1;
    }
    g_img_width    = meta.width;
    g_img_height   = meta.height;
    g_num_energies = (meta.numEnergies > 0) ? meta.numEnergies : 1;


    ArchiveClient client;
    if (!client.loadArchive(vitaPath, g_archive_samples)) {
        std::cerr << "Failed to load archive: " << vitaPath << "\n";
        return 1;
    }


    const size_t expected = static_cast<size_t>(g_img_width) * g_img_height * g_num_energies;
    if (g_archive_samples.size() < expected) {
        std::cerr << "Sample count mismatch. Have " << g_archive_samples.size()
                  << ", expected >= " << expected << "\n";
        return 1;
    }


    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(g_img_width, g_img_height);
    glutCreateWindow("Archive Viewer");

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);


    gBuffer.reset(g_img_width, g_img_height);


    processAndUploadImage();


    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
