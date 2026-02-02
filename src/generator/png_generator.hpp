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



#endif