#include <Game/Classes/Block.hpp>

namespace GameClasses {

    Block::Block() {
        this->active = true;
        this->ID = Dirt;
    }

    Block::~Block() {

    }

    void Block::setActive(bool status) {
        this->active = status;
    }

    bool Block::getActive() {
        return this->active;
    }

} // namespace GameClasses