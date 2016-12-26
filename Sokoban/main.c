//--------------------------------------------------------------------
// This is a sokoban game
// 
//--------------------------------------------------------------------
#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


// main function for test purposes.
int main(int argc, char **argv )
{
	if (argc < 2) {
		printf("not enough arguments\n");
		return 0;
	}
	level_t level;
	bool ok = lvl_load(argv[1], &level);
	
	int player_col = level.start_col;
	int player_row = level.start_row;
	int turns = 0;
	while (true) {
        lvl_display(&level, player_row, player_col);
        if (is_winning(&level)) {
            printf("you won in %d turns\n", turns);
            break;
        }
        dir_t dir;
        while (true) {
            printf("up, down, left, right (u,d,l,r)?");
            int c = getchar();
            switch (c) {
                case 'u': dir = DIR_UP;    break;
                case 'd': dir = DIR_DOWN;  break;
                case 'l': dir = DIR_LEFT;  break;
                case 'r': dir = DIR_RIGHT; break;
                default: continue;
            }
            break;
        }

        if (can_move(&level, player_row, player_col, dir, true)) {
            move(&level, &player_row, &player_col, dir);
        } else {
            printf("cannot move\n");
        }
        turns++;
    }
	printf("\n%d", ok);
	return 0;
}

