#include "png_generator.hpp"    

static const uint8_t PNG_SIGNATURE[8] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};


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

void generate_chunk_IHDR(Image &img, std::ofstream &file)
{
    IHDR_Chunk chunk;

    int channels = img.channels;
    int color_type = (channels == 3) ? 2 : 6;

    chunk.length = convert_to_big_endian_32(13);

    chunk.chunk_type[0] = 'I';
    chunk.chunk_type[1] = 'H';
    chunk.chunk_type[2] = 'D';
    chunk.chunk_type[3] = 'R';
    
    chunk.width = convert_to_big_endian_32(img.width);
    chunk.height = convert_to_big_endian_32(img.height);
    chunk.bit_depth = 8;
    chunk.color_type = color_type;
    chunk.compression = 0;
    chunk.interlace = 0;
    chunk.filter = 0;
    chunk.interlace = 0;

    std::vector<uint8_t> chunk_body;
    chunk_body.push_back('I');
    chunk_body.push_back('H');
    chunk_body.push_back('D');
    chunk_body.push_back('R');

    chunk_body.push_back((img.width >> 24) & 0xFF);
    chunk_body.push_back((img.width >> 16) & 0xFF);
    chunk_body.push_back((img.width >> 8) & 0xFF);
    chunk_body.push_back((img.width >> 0) & 0xFF);
    
    chunk_body.push_back((img.height >> 24) & 0xFF);
    chunk_body.push_back((img.height >> 16) & 0xFF);
    chunk_body.push_back((img.height >> 8) & 0xFF);
    chunk_body.push_back((img.height >> 0) & 0xFF);

    chunk_body.push_back(8);
    chunk_body.push_back(color_type);
    chunk_body.push_back(0);
    chunk_body.push_back(0);
    chunk_body.push_back(0);

    chunk.crc = convert_to_big_endian_32(crc_process(chunk_body));

    file.write(reinterpret_cast<char*>(&chunk), sizeof(chunk));
}

bool generate_chunk_IDAT(std::ofstream &file, Image &img)
{

    std::vector<uint8_t> filtered_data = filter_type_4(img, file);
    size_t out_size;
    void *src_ptr = filtered_data.data();

    void *compressed_data = tdefl_compress_mem_to_heap(src_ptr, filtered_data.size(), &out_size, TDEFL_WRITE_ZLIB_HEADER);

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

    for(size_t i = 0; i < out_size; i++)
    {
        chunk_body.push_back(*(byte_ptr + i));
    }
    
    uint32_t crc = crc_process(chunk_body);

    write_big_endian_32(file, crc);

    free(compressed_data);

    return true;

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


bool save_png(const char* filename, Image& edited_image)
{
    std::ofstream file(filename, std::ios::binary);

    if(!file.is_open())
    {
        std::cerr<<" Couldn't create the file "<<std::endl;
        return false;   
    }    

    //PNG Signature first
    file.write(reinterpret_cast<const char*>(PNG_SIGNATURE), 8);

    std::cout<<"Signature wrote onto the file"<<std::endl;


    //IHDR chunk second
    generate_chunk_IHDR(edited_image, file);

    std::cout << "IHDR chunk wrote!" << std::endl;

    //IDAT chunk third
    if(!generate_chunk_IDAT(file, edited_image))
    {
        std::cerr << "Coudnt write IDAT chunk!" << std::endl;
        return false;
    }

    //IEND chunk fourth

    generate_chunk_IEND(file);

    return true;

}
