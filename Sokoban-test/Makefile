all:
	$(CC) -std=c99 -Wall -Wextra -g main.c game.c display.c inputs.c readfiles.c ./Cglm/glmath.c ./Cglm/glutils.c ./inc/libbmpread/bmpread.c obj_loader.c -lGL -lGLEW -lglfw -lm -o ./play.bin
	chmod +x ./play.bin


console:
	$(CC) -std=c99 -Wall -Wextra -g console_test.c game.c readfiles.c ./Cglm/glmath.c ./Cglm/glutils.c -lGL -lGLEW -lglfw -lm -o ./play.bin
	chmod +x ./play.bin
	
