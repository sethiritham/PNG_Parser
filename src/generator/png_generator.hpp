#ifndef PNG_GENERATOR_HPP
#define PNG_GENERATOR_HPP

#include <vector>
#include <string>
#include <fstream>
#include <cstdint>
#include <iostream>
#include <cmath>
#include <zlib.h>
#include <cstdint>

#define MINIZ_HEADER_FILE_ONLY
#define MINIZ_NO_ZLIB_COMPATIBLE_NAMES
#include "miniz.h"
#include "parser/PNGloader.h"
#include "utils.hpp"
#include "comp_filter_png.hpp"


/**
 * @brief Saves PNG file version of the edited image.
 */
bool save_png(const char* filename, Image& edited_image);


#pragma pack(push, 1) //Leave no padding gap between variables
struct IHDR_Chunk
{
    uint32_t length;
    char chunk_type[4];
    uint32_t width; //Convert to B-endian
    uint32_t height;//Convert to B-endian
    uint8_t bit_depth;
    uint8_t color_type;
    uint8_t compression;
    uint8_t filter;
    uint8_t interlace;
    uint32_t crc;//Convert to B-endian
};
#pragma pack(pop)



#endif