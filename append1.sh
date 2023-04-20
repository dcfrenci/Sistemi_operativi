#controllo che siano due parametri
case $# in
2)	echo Numero di parametri corretto;;
*)	echo Numero di parametri errato; exit 1;;
esac
#verifico che entrambi siano file di nomi relativi semplici
for var in $*
	do
	case $var in
	*/*) 	echo Non è un nome relativo semplice; exit 2;;
	*)	echo $var è un nome relativo semplice;;
	esac
done
echo Entrambi sono nomi relativi semplici

if test -f $1 -a -r $1		#verifico se il file 1 esiste ed è leggibile
	then echo Il file esiste ed è leggibile
	else echo Il file non esiste o non è leggibile
fi
if test -f $2 -a -w $2		#verifico se il file 2 esiste ed è scrivible
	then echo Il file esiste ed è scrivibile
	else if test ! -f $2 -a -w `pwd`	#se file 2 non esiste --> directory corr w
		then echo Il file non esiste e la directory è scrivibile
		else echo Il file 2 non esiste o la directory non è scrivibile; exit 3
	fi
fi
