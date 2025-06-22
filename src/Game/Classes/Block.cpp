#include <Game/Classes/Block.hpp>

namespace GameClasses {

    Block::Block(gml::Vec3 position, EngineClasses::Shader* shader) : Cube(position, shader) {
        this->ID = Solid;
    }

    Block::~Block() {

    }

    blockID Block::getID() {
        return this->ID;
    }


} // namespace GameClasses