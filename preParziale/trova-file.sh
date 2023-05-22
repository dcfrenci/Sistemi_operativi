#!/bin/sh
#controlla numero di parametri
case $# in
1)	echo "Numero di parametri corretti"
		if test $1 = *[!/]*
			then exit 2
			echo "Il nome non è relativo semplice"
		fi;;
*)	echo "Numero di parametri errato"
	exit 1;;
esac

#verifico se il file è presente nella directory
if test $1 -f
	then echo Nome relativo semplice di $1 è `pwd`/$1
fi


