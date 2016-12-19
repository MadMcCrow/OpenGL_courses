all:
	gcc -Wall -Wextra main.c glmath.c glutils.c checker.c  -o ./play.bin -lm -lGL -lGLEW -lglut
	chmod +x ./play.bin

indent:
	chmod +x ./autoindent.sh
	./autoindent.sh

Checker:
	gcc -Wall -Wextra checker.c -o ./checker.bin -lm -lGL -lGLEW -lglut
	chmod +x ./checker.bin
	./checker.bin 3 3

