all:
	gcc -std=c99 -Wall -Wextra  vla.c parser.c -o ./play.bin -lm -lGL -lGLEW -lglut
	chmod +x ./play.bin

indent:
	chmod +x ./autoindent.sh
	./autoindent.sh
	
test:
	gcc -std=c99 -Wall -Wextra  vla.c parser.c -o ./play.bin -lm -lGL -lGLEW -lglut
	chmod +x ./play.bin
	valgrind --tool=memcheck --leak-check=yes  ./play.bin cube.obj
