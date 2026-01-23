#include <iostream>
#include "PNGloader.h"


static const uint8_t PNG_SIGNATURE[8] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};



bool PNGloader::Load(const char* filename, Image& image)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr<<"Failed to open file: "<<filename<<std::endl;
        return false;
    }

    if(!validateSignature(file))
    {
        std::cerr<<"Invalid PNG signature: "<<filename<<std::endl;
        return false;
    }

    std::cout<<"PNG signature is valid."<<std::endl;
    processChunks(file, image);

    return true;
    
}


bool PNGloader::validateSignature(std::ifstream& file)
{
    uint8_t buffer[8];
    file.read(reinterpret_cast<char*>(buffer), 8);

    if(file.gcount() != 8)
    {
        std::cerr<<"Failed to read PNG signature. (FILE SIZE TOO SMALL)"<<std::endl;
        return false;
    }

    for(int i = 0; i < 8; i++)
    {
        if(buffer[i] != PNG_SIGNATURE[i])
        {
            std::cerr<<"Invalid PNG signature."<<std::endl;
            return false;
        }
    }

    return true;
}

uint32_t PNGloader::readBigEndian32(std::ifstream& file)
{
    uint8_t bytes[4];
    file.read(reinterpret_cast<char*>(bytes), 4);
    return (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
}


bool PNGloader::processChunks(std::ifstream& file, Image& image)
{
    while(file.good())
    {
        uint32_t length = readBigEndian32(file);
        
        char type[5] = {0};
        file.read(type, 4);

        std::cout<<"Detected chunk: "<<type<<"Length: "<<length<<std::endl;

        if(std::string(type) == "IHDR")
        {
            image.width = readBigEndian32(file);
            image.height = readBigEndian32(file);

            uint8_t bitDepth = file.get();
            uint8_t colorType = file.get();

            file.ignore(3);

            if(colorType == 2) image.channels = 3;
            else if(colorType == 6) image.channels = 4;
            else
            {
                std::cerr<<"Unsupported color type: "<<colorType<<std::endl;
                return false;
            }

            std::cout << " -> Dimensions: " << image.width << "x" << image.height << std::endl;
            std::cout << " -> Channels: " << image.channels << std::endl;

            readBigEndian32(file);

        }

        else if(std::string(type) == "IDAT")
        {
            std::cout<<"Processing IDAT chunk."<<std::endl;
            
            std::vector<uint8_t> data(length);
            file.read(reinterpret_cast<char*>(data.data()), length);

            image.zlibStream.insert(image.zlibStream.end(), data.begin(), data.end());

            readBigEndian32(file);
            
        }

        else if(std::string(type) == "IEND")
        {
            std::cout<<"End of PNG file."<<std::endl;
            break;
        }
        else
        {
            file.seekg(length + 4, std::ios::cur);
        }
    }

    if (image.zlibStream.empty()) {
        std::cerr << "Error: No IDAT chunks found!" << std::endl;
        return false;
    }


    std::cout << "Total Compressed Data Size: " << image.zlibStream.size() << " bytes" << std::endl;
    return true;
}



PNGloader::PNGloader() {}
PNGloader::~PNGloader() {}

