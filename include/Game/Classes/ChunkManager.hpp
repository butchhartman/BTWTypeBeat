#ifndef GAME_CLASSES_CHUNKMANAGER_H_
#define GAME_CLASSES_CHUNKMANAGER_H_

#include <vector>
#include <game/Classes/Chunk.hpp>

namespace GameClasses {

class ChunkManager {
    private:
        gml::Vec3 lastCameraPosition;

        int renderDistance;
        int maxLoadedChunks;
        std::vector<Chunk> chunksToLoad;
        std::vector<Chunk> loadedChunks;
        std::vector<Chunk> chunksToUnload;

    public:
        ChunkManager(int renderDistance);
        ~ChunkManager();

        void update(gml::Vec3 cameraPosition);
        void renderLoadedChunks(EngineClasses::Camera sceneCamera);
    private:
       void findChunksToUnload(); 
       void unloadQueuedChunks();

        void loadQueuedChunks();
        void findChunksToLoad();
        bool hasCameraMoved(gml::Vec3 cameraPosition);
        int calculateMaxChunksFromRenderDistance();


};

} // GameClasses


#endif // GAME_CLASSES_CHUNKMANAGER_H_