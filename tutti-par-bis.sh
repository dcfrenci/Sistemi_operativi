#verifico il numero di parametri
case $# in
0)	echo Numero di parametri insufficiente; exit 1;;
*)	;;
esac
#riporto parametri in forma non aggregata
pos=0					#creo il puntatore
for var in $*
	do
	echo Parametro n°`expr $pos` = $var
	pos=`expr $pos + 1`		#incremento il contatore
	done
#riporto parametri in forma aggregata
echo `for var in $*
		do
		echo $var
		done`

