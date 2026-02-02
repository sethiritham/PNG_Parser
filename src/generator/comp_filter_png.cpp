#include "comp_filter_png.hpp"


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
            int up_left = (((row < channels) || (col == 0)) ? 0 : src[pixelIndex-stride-channels]);

            int p = left + up - up_left;
            
            int pa = std::abs(p - left);
            int pb = std::abs(p - up);
            int pc = std::abs(p - up_left);

            int closest;

            //Which neighbour is the closest to current
            if(pa <= pb && pa <= pc)
            {
                closest = left;
            }
            else if(pb <= pc && pb <= pa)
            {
                closest = up;
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
            int up_left = (((row < channels) || (col == 0)) ? 0 : src[pixelIndex-stride-channels]);

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


std::vector<uint8_t> filter_type_0(Image &img)
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

