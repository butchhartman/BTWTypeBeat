#include <algorithm>
#include <Game/Classes/ChunkManager.hpp>
#include <Engine/Classes/Shader.hpp>

namespace GameClasses {

   ChunkManager::ChunkManager(int renderDistance) : lastCameraPosition(0.0f, 0.0f, 0.0f) {
        this->renderDistance = renderDistance;
        this->maxLoadedChunks = calculateMaxChunksFromRenderDistance();
        this->loadedChunks = std::vector<Chunk>(0, Chunk(gml::Vec3(.1, .1, .1)));
   }
   
   ChunkManager::~ChunkManager() {

   }


   void ChunkManager::update(gml::Vec3 cameraPosition) {
    unloadQueuedChunks();
    loadQueuedChunks();

    if (this->hasCameraMoved(cameraPosition)) {
            findChunksToLoad();
            findChunksToUnload();
        }

    this->lastCameraPosition = cameraPosition;
   }
   
   void ChunkManager::loadQueuedChunks() {
    for (int i = 0; i < this->chunksToLoad.size(); i++) {
        // this->chunksToLoad[i].load();
        this->loadedChunks.push_back(this->chunksToLoad[i]);
        this->loadedChunks.back().load();
        // gml::Vec3 chunkPos = this->chunksToLoad[i].getPosition();
    }
    this->chunksToLoad.clear();
   }

   void ChunkManager::findChunksToLoad() {
    // TODO : Figure out a chunk loading method that doesnt require the player to be halfway inside a chunk for it to load. (Or don't as this shouldn't be a problem with render distances above 1)
    // TODO : Find alternative algorith for determining what chunk to load. I need to convert world chunk coordinates to be centered at the camera position. From there, calculate distances and queue chunk to load if the chunk is less than render distance away. 
    // alternatively, I could just iterate through the entire render distance volume and test distances that way
    int camX = (int)(this->lastCameraPosition.X()/16);
    int camY = (int)(this->lastCameraPosition.Y()/16);
    int camZ = (int)(this->lastCameraPosition.Z()/16);
    
    // TODO : check if an already loaded chunk exists at the same position. If so, do not create a new chunk
    // TODO : Clean up this unholy sequence of functions

    for (int x = -this->renderDistance; x <= this->renderDistance; x++) {
        for (int y = -this->renderDistance; y <= this->renderDistance; y++) {
            for (int z = -this->renderDistance; z <= this->renderDistance; z++) {

                if (gml::Vec3(x, y, z).magnitude() > this->renderDistance) {
                    continue;
                }

                auto pred = [=](Chunk someChunk) {
                    return (someChunk.getPosition().X()/16 == camX + x &&
                            someChunk.getPosition().Y()/16 == camY + y &&
                            someChunk.getPosition().Z()/16 == camZ + z);
                };

                auto it = std::find_if(this->loadedChunks.begin(), this->loadedChunks.end(), pred);

                if (it != this->loadedChunks.end() && this->loadedChunks.size() > 0) {
                    continue;
                }

                // TODO : There's probably a way to cache the result of this since it'll be the same every time

                gml::Vec3 newChunkPos = gml::Vec3((camX+x)*16, (camY+y)*16, (camZ+z)*16);
                Chunk newChunk = Chunk(newChunkPos);
                this->chunksToLoad.push_back(newChunk);
            }
        }
    }
   }

   void ChunkManager::renderLoadedChunks(EngineClasses::Camera sceneCamera) {
    for (int i = 0; i < this->loadedChunks.size(); i++) {
        this->loadedChunks[i].render(sceneCamera);
    }
   }

   void ChunkManager::findChunksToUnload() {
    int camX = (int)(this->lastCameraPosition.X()/16);
    int camY = (int)(this->lastCameraPosition.Y()/16);
    int camZ = (int)(this->lastCameraPosition.Z()/16);
    gml::Vec3 camP = gml::Vec3(camX, camY, camZ);

    for (int i = 0; i < this->loadedChunks.size(); i++) {
        if (((this->loadedChunks[i].getPosition()/16) - camP).magnitude() > this->renderDistance) {
            this->chunksToUnload.push_back(this->loadedChunks[i]);
            this->loadedChunks.erase(this->loadedChunks.begin()+i);
            break;
        }
    }
   }

   void ChunkManager::unloadQueuedChunks() {
    for (int i = 0; i < this->chunksToUnload.size(); i++) {
        this->chunksToUnload.back().unload();
        this->chunksToUnload.pop_back();
    }
   }

   bool ChunkManager::hasCameraMoved(gml::Vec3 cameraPosition) {
        return (cameraPosition.X() != this->lastCameraPosition.X() ||
        cameraPosition.Y() != this->lastCameraPosition.Y() ||
        cameraPosition.Z() != this->lastCameraPosition.Z());
   }

   // Returns the max amount of visible chunks of the current render distance
   int ChunkManager::calculateMaxChunksFromRenderDistance() {
    int num = 0;
    for (int x = -this->renderDistance; x <= this->renderDistance; x++) {
        for (int y = -this->renderDistance; y <= this->renderDistance; y++) {
            for (int z = -this->renderDistance; z <= this->renderDistance; z++) {
                if (gml::Vec3(x, y, z).magnitude() <= this->renderDistance) {
                    num++;
                }
            }
        }
    }
    return num;
   }

} // namespace GameClasses