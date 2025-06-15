#ifndef _ENGINE_GEOMETRY_CUBE_H_
#define _ENGINE_GEOMETRY_CUBE_H_

#include <Classes/Shader.hpp>
#include <gml.hpp>

namespace EngineClasses {

class Cube {
    private:
        static const float cubeVertices[];
        static GLuint cubeVBO;
        static GLuint cubeVAO;
        static gml::Mat4 model;

    private:
        gml::Vec3 position;

    public:
        Cube(gml::Vec3 position);
        ~Cube();
        gml::Mat4 getModelMat();
        void draw();
        static void cubeDataInit();
        static void bindVBO();
        static void bindVAO();

};

} // namespace EngineClasses

#endif //_ENGINE_GEOMETRY_CUBE_H_