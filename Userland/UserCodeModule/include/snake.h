#ifndef SNAKE_H
#define SNAKE_H
#include <libc.h>

// Direction enumeration for snake movement
typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

// Structure to represent the snake
typedef struct {
    int body[400];      // Array of blocks representing the snake's body
    int head;           //index of the head
    int tail;           //index of the tail
    int length;         // Current length of the snake
    int player;         // Player number
    Direction dir;      // Current direction of the snake
    uint32_t color;     // Color of the snake
} Snake;

void snake();

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 16

#endif // SNAKE_H