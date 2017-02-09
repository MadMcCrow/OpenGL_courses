#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "readfiles.h"
#include "Cglm/glutils.h"
#include "Cglm/glmath.h"
#include "game.h"
//#include "inputs.h"


// main function for test purposes.
int main(int argc, char **argv )
{

	level_t level;
	bool ok = lvl_load(argv[1], &level);
    player p;
	init_player(&p, &level);

	while (true) {
        update_level(&p, &level);
        lvl_display(&level, p.row, p.col);
        if (is_winning(&level)) {
            printf("you won in %d turns\n", p.turns);
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
            dir = act_player(dir, &p);
            break;
        }

        if (can_move(&level, p.row, p.col, dir, true)) {
            move(&level, &p.row, &p.col, dir);
        } else {
            printf("cannot move\n");
        }
        p.turns++;
    }
	printf("\n%d", ok);
	return 0;
}
