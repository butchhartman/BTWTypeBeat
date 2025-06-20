#ifndef ENGINE_TEXTURE_H_
#define ENGINE_TEXTURE_H_


// TODO: Currently, this class is very inflexible in choosing what texture parameters to use. Add flags to specify these.

namespace EngineClasses {

namespace TextureFlags {
    typedef enum TextureCreationFlags{
        TEXTURE_FILE_BMP_BIT = (1 << 0),
    } TextureCreationFlags;
} // namespace TextureFlags

class Texture {
    private:
        GLuint textureID;
    public:
        Texture();
        Texture(const char* filePath, TextureFlags::TextureCreationFlags flags);
        ~Texture();
        void use();
        GLuint getID();

};

} // namespace EngineClasses

#endif // ENGINE_TEXTURE_H_