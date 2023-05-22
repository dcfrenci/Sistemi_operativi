#verifico il numero di parametri
case $# in
0)	echo Numero di parametri insufficiente; exit 1;;
*)	;;
esac
#riporto parametri in forma non aggregata
for var in $*
	do
	echo $var
	done
#riporto parametri in forma aggregata 
echo `for var in $*
		do
		echo $var
		done`

