#include <PNGloader.h>

static const uint8_t PNG_SIGNATURE[8] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};

Image load_png(const char* filename)
{
    std::ifstream file;

    uint8_t sign_buffer[8];

    file.open(filename, std::ios::binary);

    file.read(reinterpret_cast<char*>(sign_buffer), 8); 

    if(file.gcount() != 8)
    {
        std::cerr<<"PNG SIGNATURE NOT VALID (TOO SMALL)"<<std::endl;
    }

    for(int i = 0 ; i < 8; i++)
    {
        if(!(sign_buffer[i] == PNG_SIGNATURE[i]))
        {
            std::cerr<<"INVALID SIGNATURE! "<<std::endl;
        }
    }

}