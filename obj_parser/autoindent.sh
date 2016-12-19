#!/bin/sh
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
