#ifndef GAME_CLASSSES_CHUNK_H_
#define GAME_CLASSSES_CHUNK_H_

#include <Game/Classes/Block.hpp>

// TODO: Currently, this chunk works rather spectacularly but has major
// performance problems. I need to figure out how to draw many chunks with low
// performance impact. (Backface culling, instancing, greedy meshing/not drawing
// unseen blocks)

// Right now I'm getting ~160 FPS rendering a single 16x16x16 chunk

// When backface culling is enabled, I get still ~160 FPS rendering a single
// 16x16x16 chunk. It is a more stable framerate, however.

// By simply not drawing unsee-able blocks, my framerate rises to ~430 rendering the single 16X16x16 chunk.

// What I'm doing here is not looking good in terms of actually working. I think I need to read up on voxel engines and how they work.

// By updating the implementation to include all chunk cube vertices in a single VBO (allowing drawing with 1 draw call), the framerate rose to 3600. It's as fast as if nothing is being drawn at all.

namespace GameClasses {

// TODO: Make chunk dimensions modifiable?

class Chunk {
    public:
        static const int chunkSize = 16;
    private:
        GLuint vertexBufferObject;
        GLuint vertexArrayObject;
        Block*** chunkBlockData;
        int visibleBlocks;
        gml::Vec3 chunkPosition;
        // temp
        EngineClasses::Shader* chunkShader;
        GLuint modelMatLocation;
        GLuint viewMatLocation;
        GLuint projMatLocation;

    public:
        Chunk(gml::Vec3 chunkPosition, EngineClasses::Shader* chunkShader);
        ~Chunk();

        void update();
        void render(EngineClasses::Camera sceneCamera);
    private:
        void createMesh();
        void createCube(int x, int y, int z, int offset);

};

} // namespace GameClasses


#endif // GAME_CLASSSES_CHUNK_H_