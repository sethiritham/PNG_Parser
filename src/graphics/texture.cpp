#include "texture.hpp"


/**
    @brief Uploads the texture to the window
    @param img The image to be uploaded
*/
GLuint UploadTexture(const Image& img)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //Removes any padding - data as it is (OPGL ASSUMES 4 BYTES PER ROW)
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Tells OPGL the exact image channels
    GLenum format = (img.channels == 4) ? GL_RGBA : GL_RGB; 

    glTexImage2D(GL_TEXTURE_2D, 0, format,
                 img.width, img.height,
                 0, format,
                 GL_UNSIGNED_BYTE,
                 img.editedPixels.data());
    
    return textureID;
}

/**
 * @brief Updates the texture after changes in editedPixels
 * @param textureID opgl specific data-type (GLuint) used for rendering
 */
void UpdateTexture(GLuint textureID,const Image& img) 
{
    glBindTexture(GL_TEXTURE_2D, textureID);
    GLenum format = (img.channels == 4) ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, format,
                 img.width, img.height,
                 0, format,
                 GL_UNSIGNED_BYTE,
                 img.editedPixels.data());
}

