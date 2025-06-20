#include <Classes/Geometry/Cube.hpp>

namespace EngineClasses {
    const float Cube::cubeVertices[] = {
        // front triangles
        0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,

        0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,

        // back triangles
        0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,

        0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,

        // right triangles
        0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, -0.5f,

        0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,

        // left triangles
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,

        // top triangles
        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,

        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,

        // bottom triangles
        0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,

        0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f

    };
    GLuint Cube::cubeVBO;
    GLuint Cube::cubeVAO;
    gml::Mat4 Cube::model = gml::Mat4::translation(0.0f, 1.0f, 0.0f);

    // Once again, had to use member initializer hack here because of the lack
    // of a no-param initializer in the gml datatypes
    Cube::Cube(gml::Vec3 position, EngineClasses::Shader* cubeShader) : position(0.0f, 0.0f, 0.0f) {
        this->position = position;
        this->cubeShader = cubeShader;

        this->modelMatPos = glGetUniformLocation(this->cubeShader->getID(), "modelMat");
        this->viewMatPos = glGetUniformLocation(this->cubeShader->getID(), "viewMat");
        this->projectionMatPos = glGetUniformLocation(this->cubeShader->getID(), "projectionMat");

    }

    Cube::~Cube() {

    }

    // Seems like the returned matrix data is going out of scope before it can get used
    gml::Mat4 Cube::getModelMat() {
        return gml::Mat4::translation(this->position.X(), this->position.Y(), this->position.Z());
    }

    void Cube::draw(EngineClasses::Camera sceneCamera) {
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBindVertexArray(cubeVAO);

        // this->cubeShader.use();
        // glUseProgram(this->cubeShaderID);
        this->cubeShader->use();

        glUniformMatrix4fv(this->modelMatPos, 1, GL_FALSE, this->getModelMat().getData());
        glUniformMatrix4fv(this->viewMatPos, 1, GL_FALSE, sceneCamera.getViewMat().getData()); 
        glUniformMatrix4fv(this->projectionMatPos, 1, GL_FALSE, sceneCamera.getPerspectiveMat().getData()); 

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    void Cube::cubeDataInit() {
        // VBO init
        glGenBuffers(1, &cubeVBO);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

        // VAO init
        glGenVertexArrays(1, &cubeVAO);
        glBindVertexArray(cubeVAO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    void Cube::bindVBO() {
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    }

    void Cube::bindVAO() {
        glBindVertexArray(cubeVAO);
    }
}
