//--------------------------------------------------------------------
// This code imports a level, using the following conventions: 
// b: boxes
// t: target
// s: start
// w: wall
//--------------------------------------------------------------------

#include "game.h"

// parsing a file containing a level. any level misbuild WILL make this crash.
bool lvl_parse(level_t* lvl, FILE *fp){
	int row = 0;
	int col = 0;
    lvl->num_walls = 0;
    lvl->num_boxes = 0;
	memset(lvl->cells, 0, sizeof(cell_t) * LVL_WIDTH * LVL_HEIGHT);
	while (row < LVL_HEIGHT) {
		int c = fgetc(fp);
		if (c == EOF) break;
		
		if (c == '\n') { row++, col = 0; continue; }
		
		if (col >= LVL_WIDTH) continue;
		cell_t* cell = lvl->cells + LVL_WIDTH * row + col;
		
		switch (c) {
			case 'w': cell->wall   = 1; lvl->num_walls++; break;
			case 'b': cell->box    = 1; lvl->num_boxes++; break;
			case 't': cell->target = 1; break;
			case 's':
			    cell->start  = 1;
			    lvl->start_row = row;
			    lvl->start_col = col;
			    break;
			case ' ': break;
			default:
			    fprintf(stderr, "garbage in level description (row %d, col %d)\n", row, col);
			    return false;
		}
		col++;
	}
    return true;
}

void lvl_display(const level_t* lvl) {
    for (int row = 0; row < LVL_HEIGHT; row++) {
        for (int col = 0; col < LVL_WIDTH; col++) {
            if (lvl->player_col == col && lvl->player_row == row) {
                printf("p");
                continue;
            }
            cell_t cell = lvl->cells[LVL_WIDTH * row + col];
            if (cell.wall)        printf("w");
            else if (cell.box)    printf("b");
            else if (cell.target) printf("t");
            else if (cell.start)  printf("s");
            else printf(" ");
        }
        printf("\n");
    }
}

bool lvl_load(const char* name, level_t* level) {
	FILE *fp = fopen(name, "r");
    strcpy(level->name, name);
	return fp && lvl_parse(level, fp);
}

void dir_to_pos(int* row, int* col, dir_t dir) {
    switch (dir) {
        case DIR_UP:    (*row)--; break; 
        case DIR_DOWN:  (*row)++; break;
        case DIR_LEFT:  (*col)--; break;
        case DIR_RIGHT: (*col)++; break;
    }
}

bool can_move(const level_t* level, int row, int col, dir_t dir, bool player) {
    int next_r = row, next_c = col;
    dir_to_pos(&next_r, &next_c, dir);
    cell_t cell = level->cells[next_r * LVL_WIDTH + next_c];
    if (cell.wall) return false;
    if (player && cell.box)
        return can_move(level, next_r, next_c, dir, false);
    if (!player && cell.box) return false;
    return true;
}

void move(level_t* level, int* row, int* col, dir_t dir){
    assert(can_move(level, *row, *col, dir, true));
    dir_to_pos(row, col, dir);
    cell_t* cell = level->cells + (*row) * LVL_WIDTH + (*col);
    if (cell->box) {
        cell->box = 0;
        int next_r = *row, next_c = *col;
        dir_to_pos(&next_r, &next_c, dir);
        level->cells[next_r * LVL_WIDTH + next_c].box = 1;
    }
}

bool is_winning(const level_t* level) {
    for (int i = 0; i < LVL_HEIGHT * LVL_WIDTH; i++) {
        if (level->cells[i].box && !level->cells[i].target) return false;
    }
    return true;
}

void init_player(player_t* p, const level_t* level){
    p->col    = level->start_col;
	p->row    = level->start_row;
	p->turns  = 0;
	p->forward = DIR_UP;
	}



bool update_level (const player_t* p , level_t* level){
    // check if there's nothing where the player will spawn.
    const cell_t* cell = level->cells + (p->row) * LVL_WIDTH + (p->col);
           if (cell->wall)         return false;
            else if (cell->box)    return false;
            else {
                level->player_row = p->row;
                level->player_col = p->col;
                return true;
}
}
