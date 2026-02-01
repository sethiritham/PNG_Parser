#include "png_generator.hpp"    
#define MINIZ_HEADER_FILE_ONLY
#include "miniz.h"

static const uint8_t PNG_SIGNATURE[8] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};


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


std::vector<uint8_t> filter_type_4(Image &img, std::ofstream &file)
{
    const std::vector<uint8_t>& src = img.editedPixels;
    int channels = img.channels;
    std::vector<uint8_t> filtered_data;

    int stride = channels*img.width;
    for(int col = 0; col < (int)img.height; col++)
    {
        filtered_data.push_back(4);
        for(int row = 0; row < stride; row++)
        {
            int pixelIndex = col*stride + row;
            uint8_t target = src[pixelIndex];
            int left = ((row < channels) ? 0 : src[pixelIndex - channels]);  
            int up = ((col == 0) ? 0 : src[pixelIndex - stride]);
            int up_left = (((row < channels) && (col == 0)) ? 0 : src[pixelIndex-stride-channels]);

            int p = left + up - up_left;

            int paeth_diff = std::min(std::min(std::abs(p-left), std::abs(p-up)), std::abs(p - up_left));

            int closest;
            if((p - paeth_diff) == up)
            {
                closest = up;
            }
            else if ((p - paeth_diff) == left)
            {
                closest = left;
            }
            else
            {
                closest = up_left;
            }

            int filtered_byte = target - closest;

            filtered_data.push_back(filtered_byte);
        }
    }

    return filtered_data;
}

std::vector<uint8_t> filter_type_3(Image &img, std::ofstream &file)
{
    const std::vector<uint8_t>& src = img.editedPixels;
    int channels = img.channels;
    std::vector<uint8_t> filtered_data;

    int stride = channels*img.width;
    for(int col = 0; col < (int)img.height; col++)
    {
        filtered_data.push_back(3);
        for(int row = 0; row < stride; row++)
        {
            int pixelIndex = col*stride + row;
            uint8_t target = src[pixelIndex];
            int left = ((row < channels) ? 0 : src[pixelIndex - channels]);  
            int up = ((col == 0) ? 0 : src[pixelIndex - stride]);
            int up_left = (((row < channels) && (col == 0)) ? 0 : src[pixelIndex-stride-channels]);

            int filtered_byte = target - up_left;

            filtered_data.push_back(filtered_byte);
        }
    }

    return filtered_data;
}


std::vector<uint8_t> filter_type_2(Image &img, std::ofstream &file)
{
    const std::vector<uint8_t>& src = img.editedPixels;
    int channels = img.channels;
    std::vector<uint8_t> filtered_data;

    int stride = channels*img.width;
    for(int col = 0; col < (int)img.height; col++)
    {
        filtered_data.push_back(2);
        for(int row = 0; row < stride; row++)
        {
            int pixelIndex = col*stride + row;
            uint8_t target = src[pixelIndex];
            int left = ((row < channels) ? 0 : src[pixelIndex - channels]);  
            int up = ((col == 0) ? 0 : src[pixelIndex - stride]);
            
            int filtered_byte = target - up;

            filtered_data.push_back(filtered_byte);
        }
    }

    return filtered_data;
}


std::vector<uint8_t> filter_type_1(Image &img, std::ofstream &file)
{
    const std::vector<uint8_t>& src = img.editedPixels;
    int channels = img.channels;
    std::vector<uint8_t> filtered_data;

    int stride = channels*img.width;
    for(int col = 0; col < (int)img.height; col++)
    {
        filtered_data.push_back(1);
        for(int row = 0; row < stride; row++)
        {
            int pixelIndex = col*stride + row;
            uint8_t target = src[pixelIndex];
            int left = ((row < channels) ? 0 : src[pixelIndex - channels]);  
            int up = ((col == 0) ? 0 : src[pixelIndex - stride]);
            
            int filtered_byte = target - left;

            filtered_data.push_back(filtered_byte);
        }
    }

    return filtered_data;
}

/**
 * @brief FilteredByte = RawByte
 */
std::vector<uint8_t> filter_type_0(Image &img, std::ofstream &file)
{
    const std::vector<uint8_t>& src = img.editedPixels;
    int channels = img.channels;
    std::vector<uint8_t> filtered_data;

    int stride = channels*img.width;
    for(int col = 0; col < (int)img.height; col++)
    {
        filtered_data.push_back(0);
        for(int row = 0; row < stride; row++)
        {
            int pixelIndex = col*stride + row;
            uint8_t target = src[pixelIndex];
            int left = ((row < channels) ? 0 : src[pixelIndex - channels]);  
            int up = ((col == 0) ? 0 : src[pixelIndex - stride]);
            
            filtered_data.push_back(target);
        }
    }

    return filtered_data;
}

void generate_chunk_IHDR(Image &img,const char* chunk_type, std::ofstream &file)
{

        int channels = img.channels;

        int color_type = (channels == 3) ? 2 : 6;

        write_big_endian_32(file, 13); //13 Byte will be the length of the DATA 

        std::vector<uint8_t> chunk_body;
        file.write("IHDR", 4);
        chunk_body.push_back('I');
        chunk_body.push_back('H');
        chunk_body.push_back('D');
        chunk_body.push_back('R');

        write_big_endian_32(file, img.width);
        chunk_body.push_back((img.width >> 24) & 0xFF);
        chunk_body.push_back((img.width >> 16) & 0xFF);
        chunk_body.push_back((img.width >> 8) & 0xFF);
        chunk_body.push_back((img.width >> 0) & 0xFF);
        
        write_big_endian_32(file, img.height);
        chunk_body.push_back((img.height >> 24) & 0xFF);
        chunk_body.push_back((img.height >> 16) & 0xFF);
        chunk_body.push_back((img.height >> 8) & 0xFF);
        chunk_body.push_back((img.height >> 0) & 0xFF);

        file.put(8);
        chunk_body.push_back(8);
        file.put(color_type);
        chunk_body.push_back(color_type);
        file.put(0);
        chunk_body.push_back(0);
        file.put(0);
        chunk_body.push_back(0);
        file.put(0);
        chunk_body.push_back(0);

        uint16_t crc =  crc_process(chunk_body);

        write_little_endian_32(file, crc);
}

bool generate_chunk_IDAT(std::ofstream &file, Image &img)
{

    std::vector<uint8_t> filtered_data = filter_type_4(img, file);
    size_t out_size;
    void *src_ptr = filtered_data.data();

    void *compressed_data = tdefl_compress_mem_to_heap(src_ptr, filtered_data.size(), &out_size, 0);

    std::vector<uint8_t> chunk_body;

    if(!compressed_data)
    {
        std::cerr<<"COULD NOT COMPRESS THE FILE"<<std::endl;
        return false;
    }

    write_big_endian_32(file, out_size);

    file.write("IDAT", 4);

    chunk_body.push_back('I');
    chunk_body.push_back('D');
    chunk_body.push_back('A');
    chunk_body.push_back('T');

    uint8_t *byte_ptr = (uint8_t*)compressed_data;

    file.write((char*)byte_ptr, out_size);

    for(int i = 0; i < out_size; i++)
    {
        chunk_body.push_back(*(byte_ptr + i));
    }
    
    uint32_t crc = crc_process(chunk_body);

    write_big_endian_32(file, crc);

}

void generate_chunk_IEND(std::ofstream &file)
{
    write_big_endian_32(file, 0);
    file.write("IEND", 4);

    std::vector<uint8_t> chunk_body;

    chunk_body.push_back('I');
    chunk_body.push_back('E');
    chunk_body.push_back('N');
    chunk_body.push_back('D');

    uint32_t crc = crc_process(chunk_body);

    write_big_endian_32(file, crc);
}

uint32_t crc_process(std::vector<uint8_t> chunk_body)
{
    uint32_t crc_table[256];

    //Generating table for CRC
    for(int i = 0; i < 256; i++)
    {
        uint32_t c = i;
        for(int j = 0; j < 8; j++)
        {
            c = (c & 1) ?
             ((c>>1) ^ 0xEDB88320) // 32 degree polynomial used in CRC calc
             : (c >> 1);
        }

        crc_table[i] = c;
    }
        uint32_t crc = 0xFFFFFFFF;

        for (size_t i = 0; i < chunk_body.size(); i++)
        {
            uint8_t index = (crc ^ chunk_body[i])&0xFF;
            crc = (crc >> 8) ^ crc_table[index];
        }

        return (crc ^ 0xFFFFFFFF);
}

bool save_png(const char* filename, Image& edited_image)
{
    std::ofstream file(filename, std::ios::binary);

    int channels = edited_image.channels;

    int color_type = (channels == 3) ? 2 : 6;

    if(!file.is_open())
    {
        std::cerr<<" Couldn't create the file "<<std::endl;
        return false;
    }    

    //PNG Signature first
    for(int i = 0; i < 8; i++)
    {
        file.write(reinterpret_cast<const char*>(PNG_SIGNATURE), 8);
        std::cout<<"Signature wrote onto the file! "<<std::endl;
    }


    //IHDR chunk second
    generate_chunk_IHDR(edited_image, "IHDR", file);

    //IDAT chunk third
    if(generate_chunk_IDAT(file, edited_image))
    {
        std::cout << "Successfully wrote IDAT chunk!" << std::endl;
    }

    //IEND chunk fourth

    


}
