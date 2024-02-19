// BOARD.h
#ifndef BOARD_H
#define BOARD_H


typedef struct
{
    double x;
    double y;
} Position;

typedef struct
{
    Position drone;
    char requried_key;
} Board;

typedef struct
{

    char charachter_for_symbols;
    short used_colors;
    int status_of_activity;
    double row;
    double col;
} KeyCharacter;

#endif // BOARD_H
