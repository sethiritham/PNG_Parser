#ifndef PNGLOADER_H
#define PNGLOADER_H

#include <vector>
#include <string>
#include <fstream>
#include <cstdint>

struct Image
{
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t channels = 0;
    std::vector<uint8_t> data;
};



class PNGloader
{
public:
    PNGloader();

    bool Load(const char* filename, Image& image);

    ~PNGloader();

private:
    bool validateSignature(std::ifstream& file);
    bool processChunks(std::ifstream& file, Image& image);
    uint32_t readBigEndian32(std::ifstream& file);
};

#endif // PNGLOADER_H