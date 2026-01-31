#ifndef PNG_GENERATOR_HPP
#define PNG_GENERATOR_HPP

#include <vector>
#include <string>
#include <fstream>
#include <cstdint>

#include "parser/PNGloader.h"

class png_generator
{
    public:
        png_generator();

        bool Save(const char* file_path, Image &edited_pixels);

        ~png_generator();
};


#endif