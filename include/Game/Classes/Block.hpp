#ifndef GAME_CLASSES_BLOCK_H_
#define GAME_CLASSES_BLOCK_H_

#include <Engine/Classes/Geometry/Cube.hpp>

namespace GameClasses {

enum blockID {
    Air,
    Dirt 
};

class Block {
    private:
        blockID ID;
        bool active;
    public:
        Block();
        ~Block();

        void setActive(bool active);
        bool getActive();

};


} // GameClasses


#endif // GAME_CLASSES_BLOCK_H_