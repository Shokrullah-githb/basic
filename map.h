
#ifndef MAP_H
#include "common.h"
#define MAP_H


typedef struct {
    char symbol[5];
} MapCell;

extern MapCell map[MAP_MAX_HEIGHT][MAP_MAX_WIDTH];
extern int map_width, map_height;

void update_and_render();

#endif // MAP_H