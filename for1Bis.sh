#!/bin/sh
#file comandi con primo esempio di for
for i in `ls -a $1`	#per tutti i file/directory della directory corrente (ATTENZIONE NON QUELLI IL CUI NOME INIZIA PER .)
do
	case $i in
	.*)	echo ELEMENTO CORRENTE $i;;	#visualizziamo il nome del file/directory
	*)	;;	#se non è nascosco non faccio niente
	esac
done
