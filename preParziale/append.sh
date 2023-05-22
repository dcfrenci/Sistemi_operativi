#controllo che siano due parametri
case $# in
1) 	if test -f $1
		then echo "Indicare i dati da scrivere in append su file 1"
		cat >> $1
		else echo "Il parametro inserito non indica un file --> verrà creato nuovo"
		cat >> $1
	fi;;
2)	echo "Numero di parametri corretto";;
*)	exit 1;;
esac
#verifico come scrivere 

if test -f $1 -a -f $2		#se entrambi sono file ed esistono
	then cat < $1 >> $2
	echo "I dati contenuti nel file1 sono stati scritti in append su file2"
	exit
fi
if test -f $2			#se file destinazione esiste
	then echo "Il primo parametro non indica un file"
	exit
fi
if test -f $1			#se file da copiare esiste
	then echo "Il secondo parametro non indica un file"
	exit
fi
echo "Entrambi i parametri passati non corrispondono a file"
