#ifndef _ENGINE_GEOMETRY_CUBE_H_
#define _ENGINE_GEOMETRY_CUBE_H_

#include <Engine/Classes/Geometry/DrawableObject.hpp>
#include <Engine/Classes/Shader.hpp>
#include <Engine/Classes/Camera.hpp>
#include <gml.hpp>

namespace EngineClasses {

class Cube : public DrawableObject {
    private:
        static const float cubeVertices[];
        static GLuint cubeVBO;
        static GLuint cubeVAO;
        static gml::Mat4 model; // TODO: Remove this

    private:
        GLuint modelMatPos;
        GLuint viewMatPos;
        GLuint projectionMatPos;

    protected:
        gml::Vec3 position;
        EngineClasses::Shader* cubeShader;

    public:
        Cube(gml::Vec3 position, EngineClasses::Shader* cubeShader);
        ~Cube();
        gml::Mat4 getModelMat();
        virtual void draw(EngineClasses::Camera sceneCamera);
        static void cubeDataInit();
        static void bindVBO();
        static void bindVAO();

};

} // namespace EngineClasses

#endif //_ENGINE_GEOMETRY_CUBE_H_