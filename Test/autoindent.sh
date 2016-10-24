if [ -e $(which uncrustify) ]; then
for i in $(ls *.c); do
	uncrustify $i --replace -l C
done
for j in $(ls *.h); do
	uncrustify $j --replace -l C
done
rm *~
echo "all files indented, have a good day!"
else
echo "sorry no uncrustify installed ( for Debian : 'sudo apt-get install uncrustify')"
exit
fi
