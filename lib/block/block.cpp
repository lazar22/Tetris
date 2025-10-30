//
// Created by Lazar on 10/29/2025.
//

#include "block.h"

#include <cstdlib>
#include <ctime>

static constexpr platform::player::block_t l_shape = {
    {
        {'1', '1', '1', '1'},
        {'0', '0', '0', '1'}
    }
};

static constexpr platform::player::block_t block_shape = {
    {
        {'0', '1', '1', '0'},
        {'0', '1', '1', '0'}
    }
};

static constexpr platform::player::block_t z_shape = {
    {
        {'1', '1', '0', '0'},
        {'0', '1', '1', '0'}
    }
};

static constexpr platform::player::block_t t_shape = {
    {
        {'0', '1', '0', '0'},
        {'1', '1', '1', '0'}
    }
};

static constexpr platform::player::block_t flat_shape = {
    {
        {'1', '1', '1', '1'},
        {'0', '0', '0', '0'}
    }
};

static bool seeded = false;

platform::player::block_t block::get_random_block(void)
{
    if (!seeded)
    {
        srand(time(nullptr));
        seeded = true;
    }

    constexpr platform::player::block_t random_blocks[5] = {l_shape, block_shape, z_shape, t_shape, flat_shape};

    const int random_index = rand() % 5;
    return random_blocks[random_index];
}

platform::player::color_t block::get_random_color(void)
{
    constexpr platform::player::color_t random_colors[5] = {
        {232, 110, 88},
        {110, 232, 88},
        {88, 110, 232},
        {232, 88, 110},
        {88, 232, 110}
    };

    const int random_index = rand() % 5;
    return random_colors[random_index];
}
