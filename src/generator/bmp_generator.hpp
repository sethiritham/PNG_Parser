#include <vector>
#include <string>
#include <fstream>
#include <cstdint>
#include <iostream>
#include <cmath>
#include <zlib.h>
#include <cstdint>

#include "parser/PNGloader.h"
#include "utils.hpp"


/**
 * @brief Saves BitMap file version of the edited image.
 */
bool save_bmp(const char* filename, Image& edited_image);