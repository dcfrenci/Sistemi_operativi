#!/bin/sh
#controllo il parametro in entrata sia uno e sia un nome relativo semplice
if test $# -ne 1
	then	echo Numero di parametri errato; exit 1;
	else	if test ! -f $1
			then echo Il file non esiste
			exit
		fi
fi
case $# in
*/*)	echo Parametri errati; exit 2;;
*)	echo Parametri corretti;;
esac
#file comandi che parte da for4.sh ma considera che la lista ottenuta dal comando cat su un file abbia un formato specifico!
#supponiamo che nel file t siano stati memorizzati i nomi di alcuni file insieme con la loro lunghezza in linee
c=0	#variabile che ci serve all'interno del for per capire se siamo su un elemento dispari (nome file) o su un elemento pari (numero di linee)
for i in `cat $1` 	#la lista usata nel for corrisponde al contenuto di un file!
do
 	c=`expr $c + 1` 	#ad ogni iterazione incrementiamo il contatore
        if test `expr $c % 2` -eq 1    	#se il contatore e' dispari
	then 
		echo elemento dispari, quindi nome di file $i
		if test -f $i		#controlliamo sia il nome di un file
		then 	cat $i		#e se si' lo visualizziamo
		fi
        else 				#se il contatore e' pari
		echo elemento pari, quindi lunghezza in linee del file appena visualizzato = $i 
        fi
done
