#ifndef GAME_CLASSES_BLOCK_H_
#define GAME_CLASSES_BLOCK_H_

#include <Engine/Classes/Geometry/Cube.hpp>

namespace GameClasses {

enum blockID {
    Air,
    Solid
};

class Block : public EngineClasses::Cube {
    private:
        blockID ID;
    public:
        Block(gml::Vec3 position, EngineClasses::Shader* shader);
        ~Block();

        blockID getID();

};


} // GameClasses


#endif // GAME_CLASSES_BLOCK_H_