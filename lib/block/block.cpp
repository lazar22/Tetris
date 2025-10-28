//
// Created by Lazar on 10/29/2025.
//

#include "block.h"

#include <cstdlib>
#include <ctime>

static constexpr block_t l_shape = {
    {
        {'1', '1', '1', '1'},
        {'0', '0', '0', '1'}
    }
};

static constexpr block_t block_shape = {
    {
        {'0', '1', '1', '0'},
        {'0', '1', '1', '0'}
    }
};

static constexpr block_t z_shape = {
    {
        {'1', '1', '0', '0'},
        {'0', '1', '1', '0'}
    }
};

static constexpr block_t t_shape = {
    {
        {'0', '1', '0', '0'},
        {'1', '1', '1', '0'}
    }
};

static constexpr block_t flat_shape = {
    {
        {'1', '1', '1', '1'},
        {'0', '0', '0', '0'}
    }
};

static bool seeded = false;

block_t block::get_random_block(void)
{
    if (!seeded)
    {
        srand(time(nullptr));
        seeded = true;
    }

    constexpr block_t random_blocks[5] = {l_shape, block_shape, z_shape, t_shape, flat_shape};

    const int random_index = rand() % 5;
    return random_blocks[random_index];
}
