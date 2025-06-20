#include <iostream>
#include <stdint.h>
#include <glad.h>
#include <Filesystem/FileReader.h>
#include <Classes/Texture.hpp>


namespace EngineClasses {

    Texture::Texture(const char* filePath, TextureFlags::TextureCreationFlags flags) {
        uint32_t texWidth;
        uint32_t texHeight;
        unsigned char* texData;

        if (flags & TextureFlags::TEXTURE_FILE_BMP_BIT) {
            texData = readBMP("a.bmp", &texWidth, &texHeight);
        } else {
            std::cout << "No file type specified; texture creation failed.\n";
        }

        glGenTextures(1, &this->textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, texData);
        glGenerateMipmap(GL_TEXTURE_2D);

        freeFileData(texData);
    }

    Texture::~Texture() {

    }

    void Texture::use() {
        glBindTexture(GL_TEXTURE_2D, this->textureID);
    }

    GLuint Texture::getID() {
        return this->textureID;
    }

} // namespace EngineClasses