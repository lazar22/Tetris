//
// Created by Lazar on 10/29/2025.
//

#ifndef BLOCK_H
#define BLOCK_H

#define PATTERN_COLUMN 2
#define PATTERN_ROW 4

typedef struct BLOCK
{
    char pattern[PATTERN_COLUMN][PATTERN_ROW];
} block_t;

class block
{
public:
    block();

    ~block() = default;

public:
    static block_t get_random_block(void);
};

#endif //BLOCK_H
