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

namespace GameClasses {

class Chunk {
    private:
        static const uint32_t chunkWidth = 16;
        // This causes a null pointer dereference when it is too large for some fucking reason
        static const uint32_t chunkHeight = 16;
        static const uint32_t chunkLength = 16;
    private:
        gml::Vec3 chunkPosition; // Position of the center of the chunk
        // 16 L x 16 W x 128 H (MC style)
        // I am getting no default constructor for block error in the chunk constructor because I am not using the : Block(...) syntax. However, I dont think I can use that syntax since it is an array. I also cant use static members to initalize this. Therefore, I think I must use the dreaded malloc (😱)
        GameClasses::Block* chunkData;//[chunkWidth][chunkHeight][chunkLength];

    public:
        Chunk(gml::Vec3 chunkPosition, EngineClasses::Shader* defaultShader);
        ~Chunk();

        void draw(EngineClasses::Camera sceneCamera);
};

} // namespace GameClasses


#endif // GAME_CLASSSES_CHUNK_H_