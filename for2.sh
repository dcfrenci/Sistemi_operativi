#!/bin/sh
#file comandi con primo esempio di for
for i in `ls -a $1`	#per tutti i file/directory della directory corrente (ATTENZIONE NON QUELLI IL CUI NOME INIZIA PER .)
do
	case $i in
	p*)	if test -f $i
			then	echo Il file $i contiene
				cat $i
			else	echo Il nome $i corrisponde ad und directory
		fi;;
	*)	;;	#se non è nascosco non faccio niente
	esac
done
