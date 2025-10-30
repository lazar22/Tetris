//
// Created by Lazar on 10/29/2025.
//

#ifndef BLOCK_H
#define BLOCK_H

#include "game.h"

class block
{
public:
    block();

    ~block() = default;

public:
    static platform::player::block_t get_random_block(void);

    static platform::player::color_t get_random_color(void);
};

#endif //BLOCK_H
