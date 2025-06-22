#include <Game/Classes/Chunk.hpp>


namespace GameClasses {

    Chunk::Chunk(gml::Vec3 chunkPosition, EngineClasses::Shader* defaultShader) : chunkPosition(0.0f, 0.0f, 0.0f) {

        this->chunkPosition = chunkPosition;

        chunkData = (Block*)malloc(sizeof(Block) * chunkWidth * chunkHeight * chunkLength);

        for (int x = 0; x < chunkWidth; x++) {
            for (int y = 0; y < chunkHeight; y++) {
                for (int z = 0; z < chunkLength; z++) {
                    *(chunkData + x * chunkHeight * chunkLength + y * chunkLength + z) = Block(gml::Vec3(x + this->chunkPosition.X(), y + this->chunkPosition.Y(), z + this->chunkPosition.Z()), defaultShader);
                }
            }
        }

    }

    Chunk::~Chunk() {
        free(chunkData);
    }

    void Chunk::draw(EngineClasses::Camera sceneCamera) {
        for (int x = 0; x < chunkWidth; x++) {
            for (int y = 0; y < chunkHeight; y++) {
                for (int z = 0; z < chunkLength; z++) {

                    Block a = *(chunkData + x * chunkHeight * chunkLength + y * chunkLength + z);

                    if (x-1 < 0 ||
                        x+1 > chunkHeight-1 ||
                        y-1 < 0 ||
                        y+1 > chunkHeight-1 ||
                        z-1 < 0 ||
                        z+1 > chunkLength-1) {

                            a.draw(sceneCamera);
                            continue;
                        }

                    Block above = *(chunkData + x * chunkHeight * chunkLength + (y+1) * chunkLength + z);
                    Block below = *(chunkData + x * chunkHeight * chunkLength + (y-1)* chunkLength + z);

                    Block right = *(chunkData + (x+1) * chunkHeight * chunkLength + y * chunkLength + z);


                    Block left = *(chunkData + (x-1) * chunkHeight * chunkLength + y * chunkLength + z);

                    Block front = *(chunkData + x * chunkHeight * chunkLength + y * chunkLength + (z+1));

                    Block back = *(chunkData + x * chunkHeight * chunkLength + y * chunkLength + (z-1));


                    if (above.getID() == Air ||
                        below.getID() == Air||
                        right.getID() == Air||
                        left.getID() == Air||
                        front.getID() == Air||
                        back.getID() == Air) {
                            a.draw(sceneCamera);
                        }

                }
            }
        }
    }

} // namespace GameClasses