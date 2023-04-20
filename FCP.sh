#controllo i parametri
if test $# -ne 1
	then echo Numero di parametri errato; exit 1;
fi
#controllo che il parametro sia un nome relativo assoluto di una directory
case $1 in
*/*)	if test -d $1 -a -x $1
		then 	echo Il parametro corrisponde ad una directory traversabile
	fi;;
*)	exit 2;;
esac
PATH=`pwd`:$PATH
export PATH
file-dir.sh $1
