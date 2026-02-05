#include <PNGloader.h>
#include "png_generator.hpp"
#include "utils.hpp"
#include <cstring>

static const uint8_t PNG_SIGNATURE[8] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};

Image load_png(const char* filename)
{
    std::ifstream file;
    Image img;

    uint8_t sign_buffer[8];

    file.open(filename, std::ios::binary);

    file.read(reinterpret_cast<char*>(sign_buffer), 8); 

    if(file.gcount() != 8)
    {
        std::cerr<<"PNG SIGNATURE NOT VALID (TOO SMALL)"<<std::endl;
        return {};
    }

    for(int i = 0 ; i < 8; i++)
    {
        if(!(sign_buffer[i] == PNG_SIGNATURE[i]))
        {
            std::cerr<<"INVALID SIGNATURE! "<<std::endl;
            return {};
        }
    }

    if(memcmp(sign_buffer, PNG_SIGNATURE, 8) != 0)
    {
        std::cout<<"INVALID PNG SIGNATURE"<<std::endl;
        return {};
    }

    IHDR_Chunk ihdr;

    file.read(reinterpret_cast<char*>(&ihdr), sizeof(ihdr));

    ihdr.length = convert_to_little_endian_32(ihdr.length);
    ihdr.width = convert_to_little_endian_32(ihdr.width);
    ihdr.height = convert_to_little_endian_32(ihdr.height);
    ihdr.crc = convert_to_little_endian_32(ihdr.crc);

    char chunk_type[4];
    uint32_t chunk_length;
    std::vector<uint8_t> compressed_data;


    while(true)
    {

        file.read(reinterpret_cast<char*>(&chunk_length), 4);
        chunk_length = convert_to_little_endian_32(chunk_length);
        file.read(chunk_type, 4);

        if(memcmp(chunk_type, "IDAT", 4) == 0)
        {
            //IDAT LOGIC  
            std::vector<uint8_t> data(chunk_length);
            uint32_t idat_crc;

            file.read(reinterpret_cast<char*>(data.data()), chunk_length);
            file.read(reinterpret_cast<char*>(&idat_crc), 4);

            idat_crc = convert_to_little_endian_32(idat_crc);

            compressed_data.insert(compressed_data.end(), data.begin(), data.end());

            continue;
        }
        else if(memcmp(chunk_type, "IEND", 4) == 0)
        {
            //IEND LOGIC 
            break;
        }
        else
        {
            std::cout<<"SKIPPING THIS CHUNK"<<std::string(chunk_type, 4)<<std::endl;
            file.seekg(chunk_length + 4, std::ios::cur);
            continue;
        }
    }

}