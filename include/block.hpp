#ifndef BLOCK_H
#define BLOCK_H

#include <glm/glm.hpp>

class Block {
public:

    enum BlockID {
        AIR     = 0,
        GRASS   = 1
    };

    bool transparent;
    glm::vec2 texure_location;

    void block_init();

};

#endif
