#include<vector>
#include<cstdint>
#include"parser/PNGloader.h"


/**
 * @brief paeth filtering
 */
std::vector<uint8_t> filter_type_4(Image &img, std::ofstream &file);


/**
 * @brief assumes up_left pixel is the nearest neighbour, target = raw - up_left
 */
std::vector<uint8_t> filter_type_3(Image &img, std::ofstream &file);


/**
 * @brief assumes up pixel is the nearest neighbour, target = raw - up
 */
std::vector<uint8_t> filter_type_2(Image &img, std::ofstream &file);

/**
 * @brief assumes left pixel is the nearest neighbour, target = raw - left
 */
std::vector<uint8_t> filter_type_1(Image &img, std::ofstream &file);

/**
 * @brief No filtering
 */
std::vector<uint8_t> filter_type_0(Image &img, std::ofstream &file);