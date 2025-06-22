#include <Engine/Classes/Geometry/Cube.hpp>
#include <Engine/Classes/Texture.hpp>
#include <Engine/Classes/Geometry/TexturedCube.hpp>


namespace EngineClasses {

    const float TexturedCube::texturedCubeVertices[] = {
        // front triangles
        0.5f, 0.5f, 0.5f,   1.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f,  1.0f, 0.0f, // CCW 

        0.5f, 0.5f, 0.5f,   1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // CCW

        // back triangles
        0.5f, 0.5f, -0.5f,   0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // CW

        0.5f, 0.5f, -0.5f,   0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 
        -0.5f, 0.5f, -0.5f,  1.0f, 1.0f, // CW

        // right triangles
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, 0.5f,  0.0f, 0.0f, // CCW

        0.5f, 0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, 0.5f, 0.5f,   0.0f, 1.0f,
        0.5f, -0.5f, 0.5f,  0.0f, 0.0f, // CCW

        // left triangles
        -0.5f, 0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f,  1.0f, 0.0f, // CCW

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f,  1.0f, 1.0f, // CCW

        // top triangles
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f,  1.0f, 0.0f, // CCW

        0.5f, 0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f,  0.0f, 0.0f, // CCW

        // bottom triangles
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f,  1.0f, 0.0f, 
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // CCW

        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f // CCW
    };
    GLuint TexturedCube::texturedCubeVBO;
    GLuint TexturedCube::texturedCubeVAO;

    TexturedCube::TexturedCube(gml::Vec3 position, EngineClasses::Shader*
    cubeShader, Texture* cubeTexture) : Cube(position, cubeShader) {
        this->cubeTexture = cubeTexture;
        this->modelMatPos = glGetUniformLocation(this->cubeShader->getID(), "modelMat");
        this->viewMatPos = glGetUniformLocation(this->cubeShader->getID(), "viewMat");
        this->projectionMatPos = glGetUniformLocation(this->cubeShader->getID(), "projectionMat");
    }

    TexturedCube::~TexturedCube() {

    }

    void TexturedCube::draw(Camera sceneCamera) {
        glBindBuffer(GL_ARRAY_BUFFER, TexturedCube::texturedCubeVBO);
        glBindVertexArray(TexturedCube::texturedCubeVAO);

        // this->cubeShader.use();
        // glUseProgram(this->cubeShaderID);
        this->cubeTexture->use();
        this->cubeShader->use();

        glUniformMatrix4fv(this->modelMatPos, 1, GL_FALSE, this->getModelMat().getData());
        glUniformMatrix4fv(this->viewMatPos, 1, GL_FALSE, sceneCamera.getViewMat().getData()); 
        glUniformMatrix4fv(this->projectionMatPos, 1, GL_FALSE, sceneCamera.getPerspectiveMat().getData()); 

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    void TexturedCube::texturedCubeDataInit() {
        // VAO init
       glGenBuffers(1, &TexturedCube::texturedCubeVBO); 
       glBindBuffer(GL_ARRAY_BUFFER, TexturedCube::texturedCubeVBO);
       glBufferData(GL_ARRAY_BUFFER, sizeof(TexturedCube::texturedCubeVertices), TexturedCube::texturedCubeVertices, GL_STATIC_DRAW);

       // VAO init
       glGenVertexArrays(1, &TexturedCube::texturedCubeVAO);
       glBindVertexArray(TexturedCube::texturedCubeVAO);
       glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
       glEnableVertexAttribArray(0);
       glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
       glEnableVertexAttribArray(1);
    }

} // namespace EngineClasses