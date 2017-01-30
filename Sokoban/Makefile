all:
	$(CC) -std=c99 -Wall -Wextra -g main.c game.c display.c inputs.c readfiles.c ./Cglm/glmath.c ./Cglm/glutils.c -lGL -lGLEW -lglfw -lm -o ./play.bin
	chmod +x ./play.bin

indent:
	if [ -e $(which uncrustify) ]; then
	if [ -e ~/.uncrustify.cfg ]; then
	for i in $(ls *.c); do
		uncrustify $i --replace --no-backup -l C -c ~/.uncrustify.cfg
	done
	for j in $(ls *.h); do
		uncrustify $j --replace --no-backup -l C -c ~/.uncrustify.cfg
	done
	echo "all files indented, have a good day!"
	else
	echo "no CFG file please add ~/.uncrustify.cfg"
	fi
	else
	echo "sorry no uncrustify installed ( for Debian : 'sudo apt-get install uncrustify')"
	exit
	fi
	
test:
	$(CC) -std=c99 -Wall -Wextra  main.c game.c readfiles.c ./Cglm/glmath.c ./Cglm/glutils.c -o ./play.bin -lGLEW -lglfw -lGL
	chmod +x ./play.bin
	valgrind --tool=memcheck --leak-check=yes  ./play.bin level1.lvl
	
display:
	$(CC) -std=c11 -Wall -Wextra display.c  game.c readfiles.c -g -o ./display.bin ./Cglm/glmath.c ./Cglm/glutils.c -lGL -lGLEW -lglfw -lm
