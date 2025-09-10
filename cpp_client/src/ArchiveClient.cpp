#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

class ArchiveClient {
public:
    bool loadArchive(const std::string& path, std::vector<uint16_t>& ioArchive) {
        std::ifstream file(path, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Failed to open archive: " << path << std::endl;
            return false;
        }

        file.seekg(0, std::ios::end);
        size_t size = file.tellg();
        file.seekg(0, std::ios::beg);

        ioArchive.resize(size / 2);
        file.read(reinterpret_cast<char*>(ioArchive.data()), size);
        file.close();
        return true;
    }
};
