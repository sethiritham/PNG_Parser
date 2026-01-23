#include <iostream>
#include "parser/PNGloader.h"

PNGloader pngLoader;
Image image;

int main()
{
    if (pngLoader.Load("sculpture.png", image))
    {
        std::cout << "PNG loaded successfully.\n";
        std::cout << "Width: " << image.width << "\n";
        std::cout << "Height: " << image.height << "\n";
        std::cout << "Channels: " << image.channels << "\n";
        std::cout << "Data size: " << image.data.size() << " bytes\n";
    }
    else
    {
        std::cout << "Failed to load PNG.\n";
    }
}