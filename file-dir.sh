#mi posiziono nella directory corretta
cd $1
file=0
dir=0
for elem in `ls $1`
	do
	if test -f $elem
		then	echo [F] $elem
			file=`expr $file + 1`
		else	echo \[D\] $elem
			dir=`expr $dir + 1`
	fi
	done
echo File stampati $file
echo Directory stampate $dir
