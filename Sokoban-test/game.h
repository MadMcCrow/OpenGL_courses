//--------------------------------------------------------------------
// This code imports a level, using the following conventions: 
// b: boxes
// t: target
// s: start
// w: wall
//--------------------------------------------------------------------
#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define LVL_WIDTH  25
#define LVL_HEIGHT 25

typedef struct {
    int wall    : 1;
    int box     : 1;
    int start   : 1;
    int target  : 1;
} cell_t;

typedef struct { 
	char name[25];
	cell_t cells[LVL_WIDTH * LVL_HEIGHT];
	int start_row, start_col;
	int num_walls;
    int num_boxes; //needed for the rendering process.
    int player_row, player_col;
} level_t;

typedef enum {
    DIR_UP,
    DIR_LEFT,
    DIR_DOWN,
    DIR_RIGHT
} dir_t;

typedef struct {
    char name[25];
    int turns;
    int points;
    int row, col;
    dir_t forward;
} player_t;

// parsing a file containing a level. any level misbuild WILL make this crash.
bool lvl_parse(level_t* level, FILE *fp);

void lvl_display(const level_t* level);

bool lvl_load(const char* name, level_t* level);

void dir_to_pos(int* row, int* col, dir_t dir);

bool can_move(const level_t* level, int row, int col, dir_t dir, bool player);

void move(level_t* level, int* row, int* col, dir_t dir);

bool is_winning(const level_t* level);

void init_player(player_t* p, const level_t* level);

dir_t act_player(dir_t dir, player_t* p);

bool update_level (const player_t* p , level_t* level);

#endif
