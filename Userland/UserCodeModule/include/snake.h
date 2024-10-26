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
    Point body[400];  // Array of blocks representing the snake's body
    int length;       // Current length of the snake
    Direction dir;    // Current direction of the snake
} Snake;


// // Function prototypes
// void init_snake(Snake *snake, int initial_length, Point start_position);
// void move_snake(Snake *snake);
// char check_collision(Snake *snake, int grid_width, int grid_height);
// void change_direction(Snake *snake, Direction new_direction);
// void grow_snake(Snake *snake);

#endif // SNAKE_H