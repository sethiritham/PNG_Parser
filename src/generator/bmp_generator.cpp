#include "bmp_generator.hpp"


bool save_bmp(const char* filename, Image& edited_image)
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
