all:
	gcc -Wall -Wextra main.c glmath.c glutils.c checker.c  -o ./play.bin -lm -lGL -lGLEW -lglut
	chmod +x ./play.bin

indent:
	chmod +x ./autoindent.sh
	./autoindent.sh

checker:
	gcc -Wall -Wextra -g  main.c  glmath.c glutils.c checker.c  -o ./play.bin -lm -lGL -lGLEW -lglut
	chmod +x ./play.bin
	 valgrind ./play.bin 13 13

