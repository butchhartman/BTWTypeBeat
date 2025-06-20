#ifndef _ENGINE_GEOMETRY_CUBE_H_
#define _ENGINE_GEOMETRY_CUBE_H_

#include <Classes/Geometry/DrawableObject.hpp>
#include <Classes/Shader.hpp>
#include <Classes/Camera.hpp>
#include <gml.hpp>

namespace EngineClasses {

class Cube : public DrawableObject {
    private:
        static const float cubeVertices[];
        static GLuint cubeVBO;
        static GLuint cubeVAO;
        static gml::Mat4 model;

    private:
        GLuint modelMatPos;
        GLuint viewMatPos;
        GLuint projectionMatPos;
        gml::Vec3 position;

    protected:
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