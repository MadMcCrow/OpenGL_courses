for i in $(ls|grep '.c');do
uncrustify $i --replace -l C
done
for j in $(ls|grep '.h');do
uncrustify $j --replace -l C
done
echo "all files indented, have a good day!"
