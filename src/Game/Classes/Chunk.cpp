#include <Game/Classes/Chunk.hpp>

const float cubeVertices[108] = {
    // front triangles
    0.5f, 0.5f, 0.5f,   
    -0.5f, -0.5f, 0.5f, 
    0.5f, -0.5f, 0.5f,  // CCW

    0.5f, 0.5f, 0.5f,   
    -0.5f, 0.5f, 0.5f, 
    -0.5f, -0.5f, 0.5f, // CCW

    // back triangles
    0.5f, 0.5f, -0.5f,   
    0.5f, -0.5f, -0.5f,  
    -0.5f, -0.5f, -0.5f, // CW

    0.5f, 0.5f, -0.5f,   
    -0.5f, -0.5f, -0.5f, 
    -0.5f, 0.5f, -0.5f,  // CW

    // right triangles
    0.5f, -0.5f, -0.5f, 
    0.5f, 0.5f, -0.5f,  
    0.5f, -0.5f, 0.5f, // CCW

    0.5f, 0.5f, -0.5f,  
    0.5f, 0.5f, 0.5f,   
    0.5f, -0.5f, 0.5f, // CCW

    // left triangles
    -0.5f, 0.5f, -0.5f,  
    -0.5f, -0.5f, -0.5f, 
    -0.5f, -0.5f, 0.5f,  // CCW

    -0.5f, 0.5f, -0.5f, 
    -0.5f, -0.5f, 0.5f, 
    -0.5f, 0.5f, 0.5f,  // CCW

    // top triangles
    0.5f, 0.5f, -0.5f, 
    -0.5f, 0.5f, 0.5f, 
    0.5f, 0.5f, 0.5f,  // CCW

    0.5f, 0.5f, -0.5f,  
    -0.5f, 0.5f, -0.5f, 
    -0.5f, 0.5f, 0.5f,  // CCW

    // bottom triangles
    0.5f, -0.5f, -0.5f, 
    0.5f, -0.5f, 0.5f,  
    -0.5f, -0.5f, 0.5f, // CCW

    0.5f, -0.5f, -0.5f,  
    -0.5f, -0.5f, 0.5f,  
    -0.5f, -0.5f, -0.5f, // CCW
};



namespace GameClasses {
    EngineClasses::Shader Chunk::chunkShader;

    Chunk::Chunk(gml::Vec3 chunkPosition) : chunkPosition(0.0f, 0.0f, 0.0f) {
        this->chunkPosition = chunkPosition;
        this->loaded = false;
    }

    Chunk::~Chunk() {

    }

    void Chunk::initChunkStaticData() {
        Chunk::chunkShader = EngineClasses::Shader("Shaders/vertexShader.vert", "Shaders/fragmentShaderGreen.frag");
    }

    void Chunk::update() {

    }

    void Chunk::render(EngineClasses::Camera sceneCamera) {
        Chunk::chunkShader.use();
        glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferObject);
        glBindVertexArray(this->vertexArrayObject);

        glUniformMatrix4fv(this->modelMatLocation, 1, GL_FALSE, gml::Mat4::translation(this->chunkPosition.X(), this->chunkPosition.Y(), this->chunkPosition.Z()).getData());
        glUniformMatrix4fv(this->viewMatLocation, 1, GL_FALSE, sceneCamera.getViewMat().getData());
        glUniformMatrix4fv(this->projMatLocation, 1, GL_FALSE, sceneCamera.getPerspectiveMat().getData());

        glDrawArrays(GL_TRIANGLES, 0, this->visibleBlocks*36); // There are 36 vertices in 1 cube
    }

    void Chunk::load() {
        this->modelMatLocation = glGetUniformLocation(Chunk::chunkShader.getID(), "modelMat");
        this->viewMatLocation = glGetUniformLocation(Chunk::chunkShader.getID(), "viewMat");
        this->projMatLocation = glGetUniformLocation(Chunk::chunkShader.getID(), "projectionMat");

        // Create buffer but do nothing with it since the data it contains will vary
        glGenBuffers(1, &(this->vertexBufferObject));
        // Bind the buffer so that the VAO knows which buffer to read data from
        glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferObject); 

        // Create and initialize VAO because it will stay constant
        glGenVertexArrays(1, &(this->vertexArrayObject));
        glBindVertexArray(this->vertexArrayObject);
        // A buffer MUST be bound before calling this, else the buffer which the VAO reads from will be undefined :(
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // array of T*** holds data of type T**
        // The new keyword automatically calls the initializer function for each member so this array is populated
        this->chunkBlockData = new Block**[Chunk::chunkSize];

        for (int i = 0; i < Chunk::chunkSize; i++) {
            this->chunkBlockData[i] = new Block*[Chunk::chunkSize];
            for (int j = 0; j < Chunk::chunkSize; j++) {
                this->chunkBlockData[i][j] = new Block[Chunk::chunkSize];
            }
        }

        this->createMesh();

        this->loaded = true;
    }

    void Chunk::unload() {
        glDeleteBuffers(1, &(this->vertexBufferObject));
        glDeleteVertexArrays(1, &(this->vertexArrayObject));

        for (int i = 0; i < Chunk::chunkSize; i++) {
            for (int j = 0; j < Chunk::chunkSize; j++) {
                delete[] this->chunkBlockData[i][j];
            }
            delete[] this->chunkBlockData[i];
        }
        delete[] this->chunkBlockData;
    }

    gml::Vec3 Chunk::getPosition() {
        return this->chunkPosition;
    }

    void Chunk::createMesh() {
        glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferObject); // the create cube assumes this buffer is still bound
        this->visibleBlocks = 0;
        int blockOffset = 0;
        // I need to know how many blocks are visible before I can allocate my vertex buffer. The only way I can think of to do this required traversiing the chunk data array twice (slow, but probably faster than calling new or malloc every iteration).
        
        // TODO: Figure out how to avoid writing these loops twice. At least, if possible, I want the loop logic to be the same but the assignment (visibleBlocks++, this->createCube) calls to be flexible

        for (int x = 0; x < Chunk::chunkSize; x++) {
            for (int y = 0; y < Chunk::chunkSize; y++) {
                for (int z = 0; z < Chunk::chunkSize; z++) {
                    if (this->chunkBlockData[x][y][z].getActive() == false) {
                        // do not include in mesh, block is inactive
                        continue;
                    } 

                    // If the x, y, z coordinates wont go out of range by sub/adding 1, and if all surrounding blocks are active, then skip cataloguing/drawing this block
                    if (
                        (x+1 < Chunk::chunkSize && x-1 >= 0 &&
                        y+1 < Chunk::chunkSize && y-1 >= 0 &&
                        z+1 < Chunk::chunkSize && z-1 >= 0)
                            &&
                        (this->chunkBlockData[x+1][y][z].getActive() == true &&
                        this->chunkBlockData[x][y+1][z].getActive() == true &&
                        this->chunkBlockData[x][y][z+1].getActive() == true &&
                        this->chunkBlockData[x-1][y][z].getActive() == true &&
                        this->chunkBlockData[x][y-1][z].getActive() == true &&
                        this->chunkBlockData[x][y][z-1].getActive() == true) ) {
                            continue;
                    }

                    
                    this->visibleBlocks++;
                }
            }
        }

        // Allocate buffer space to be written to with glBufferSubData (from create cube member function)
        glBufferData(GL_ARRAY_BUFFER, visibleBlocks * sizeof(cubeVertices), NULL, GL_DYNAMIC_DRAW);


        for (int x = 0; x < Chunk::chunkSize; x++) {
            for (int y = 0; y < Chunk::chunkSize; y++) {
                for (int z = 0; z < Chunk::chunkSize; z++) {
                    if (this->chunkBlockData[x][y][z].getActive() == false) {
                        // do not include in mesh, block is inactive
                        continue;
                    }

                    // If the x, y, z coordinates wont go out of range by sub/adding 1, and if all surrounding blocks are active, then skip cataloguing/drawing this block
                    if (
                        (x+1 < Chunk::chunkSize && x-1 >= 0 &&
                        y+1 < Chunk::chunkSize && y-1 >= 0 &&
                        z+1 < Chunk::chunkSize && z-1 >= 0)
                            &&
                        (this->chunkBlockData[x+1][y][z].getActive() == true &&
                        this->chunkBlockData[x][y+1][z].getActive() == true &&
                        this->chunkBlockData[x][y][z+1].getActive() == true &&
                        this->chunkBlockData[x-1][y][z].getActive() == true &&
                        this->chunkBlockData[x][y-1][z].getActive() == true &&
                        this->chunkBlockData[x][y][z-1].getActive() == true) ) {
                            continue;
                    }

                    this->createCube(x, y, z, blockOffset);
                    blockOffset++;
                }
            }
        }
    }

    void Chunk::createCube(int x, int y, int z, int offset) {
        float thisCubeVertices[108];
        for (int i = 0; i < 108; i += 3) { // stride of 3
            // Subtract the chunk size so that the chunk's position describes the position of its center instead of it's bottom left block
            thisCubeVertices[i] = cubeVertices[i] + (float)x - chunkSize/2.0f; 
            thisCubeVertices[i+1] = cubeVertices[i+1] + (float)y - chunkSize/2.0f; 
            thisCubeVertices[i+2] = cubeVertices[i+2] + (float)z - chunkSize/2.0f;
        }

        glBufferSubData(GL_ARRAY_BUFFER, offset * sizeof(cubeVertices), sizeof(cubeVertices), thisCubeVertices);

    }



} // namespace GameClasses