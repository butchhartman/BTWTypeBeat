#ifndef ENGINE_GEOMETRY_TEXTURED_CUBE
#define ENGINE_GEOMETRY_TEXTURED_CUBE

namespace EngineClasses {

class TexturedCube : Cube {
    private:
        static const float texturedCubeVertices[];
        static GLuint texturedCubeVBO;
        static GLuint texturedCubeVAO;
    private:
        Texture* cubeTexture;
        GLuint modelMatPos;
        GLuint viewMatPos;
        GLuint projectionMatPos;
    public:
        TexturedCube(gml::Vec3 position, EngineClasses::Shader* cubeShader,
        Texture* cubeTexture);
        ~TexturedCube();

        void draw(Camera sceneCamera) override;
        
        static void texturedCubeDataInit();
        // TODO: override bind VAO and bind VBO
};

} // namespace EngineClasses

#endif //ENGINE_GEOMETRY_TEXTURED_CUBE  