#include "editor.hpp"

/**
   @brief handles contrast, saturation, brightness logic
 */
void processImage(Image& img, int brightness, float contrast, float saturation)
{
    img.editedPixels = img.pixels;
    int bytesPerPixel = img.channels;
    float l_saturation;
    float m_contrast = 128;
    for(int i = 0; i < (int)img.editedPixels.size(); i+=bytesPerPixel)
    {
        int colorChannels = (bytesPerPixel == 4) ? 3 : bytesPerPixel;
        l_saturation = 0.299f*img.editedPixels[i] + 0.587f*img.editedPixels[i+1] + 0.114f*img.editedPixels[i+2];
        for(int c = 0; c < colorChannels; c++)
        {
            int value = img.editedPixels[i + c] + brightness;
            value = clamp_value_255(value);
            value = (int)(value - m_contrast) * contrast + m_contrast;
            value = clamp_value_255(value);
            value = (int)(value - l_saturation)*saturation + l_saturation;
            value = clamp_value_255(value);
            img.editedPixels[i+c] = value;
        }
    }
}