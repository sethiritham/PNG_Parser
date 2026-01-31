#include "png_generator.hpp"    

static const uint8_t PNG_SIGNATURE[8] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};

void write_little_endian_16(std::ofstream& file, uint16_t value) {
    uint8_t bytes[2];
    bytes[0] = (value >> 0) & 0xFF;
    bytes[1] = (value >> 8) & 0xFF;
    file.write(reinterpret_cast<char*>(bytes), 2);
}

void write_little_endian_32(std::ofstream& file, uint32_t value)
{
    uint8_t bytes[4];
    bytes[0] = (value >> 0) & 0xFF; // Least significant byte first (LITTLE ENDIAN)
    bytes[1] = (value >> 8) & 0xFF;
    bytes[2] = (value >> 16) & 0xFF;
    bytes[3] = (value >> 24) & 0xFF;
    file.write(reinterpret_cast<char*>(bytes), 4);
}

bool SaveBMP(const char* filename, Image& edited_image)
{
    std::ofstream file(filename, std::ios::binary);

    if(!file.is_open())
    {
        std::cerr<<" Couldn't create the file "<<std::endl;
        return false;
    }

    uint32_t padding_amount = (4 - (edited_image.width * 3) % 4 ) % 4;
    uint32_t data_offset = 54; //0x36
    uint32_t header_size = 40; //0x28
    uint32_t file_size = data_offset + (edited_image.width  * 3  + padding_amount) * edited_image.height;
    
    file.write("BM", 2); //FILE SIGNATURE
    
    write_little_endian_32(file, file_size); 
    write_little_endian_32(file, 0); // reserved 
    write_little_endian_32(file, data_offset); // where the pixel data starts
    write_little_endian_32(file, header_size); 

    write_little_endian_32(file, edited_image.width);
    write_little_endian_32(file, -(int32_t)edited_image.height);


    file.put(1); file.put(0); //0x10
    file.put(24); file.put(0); //0x18

    write_little_endian_32(file, 0);// Compression (NONE)   
    write_little_endian_32(file, 0);// Image Size (No compression, can be set to 0)
    write_little_endian_32(file, 0);// X Pixels Per Meter (Unused, set to 0).
    write_little_endian_32(file, 0);// Y Pixels Per Meter (Unused, set to 0).    
    write_little_endian_32(file, 0);// Colors in Pallete (0 default)
    write_little_endian_32(file, 0);// Important colours (0 = All)

    // IMAGE DATA
    const std::vector<uint8_t>& src = edited_image.editedPixels;
    int channels = edited_image.channels;

    for(int y = 0; y < (int)edited_image.height; y++)
    {
        for(int x = 0; x < (int)edited_image.width; x++)
        {
            int index = (y* (int)edited_image.width + x)*channels;
            int r = src[index];
            int g = src[index + 1];
            int b = src[index + 2];

            file.put(b);
            file.put(g);
            file.put(r);
        }

        for(int p = 0; p < (int)padding_amount; p++)
        {
            file.put(0);
        }
    }

    file.close();

    std::cout<<"File writing done! "<<std::endl;
    return true;

}


