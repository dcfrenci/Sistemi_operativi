#verfico il numero di parametri
case $# in
1)	echo "Numero corretto di parametri";;
*)	echo "Numero errato di parametri"
	exit;;
esac
#stampo il parametro passato
case $1 in
/*)	echo "Il nome è assoluto"
			if test -f $1
				then echo "E' un file"
			fi
			if test -d $1
				then echo "E' una directory"
				if test -x $1
					then echo "E' traversabile"
				else echo "Non è traversabile"
				fi
			fi;;
*/*)		echo "Il nome è relativo";;
*)		echo "Il nome è relativo semplice";;
esac


