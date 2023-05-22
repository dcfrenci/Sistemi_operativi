#!/bin/sh
#file comandi LeggiEMostra.sh 
#CONTROLLO SU PRESENZA DI UN SINGOLO PARAMETRO FATTO IN MODO DIVERSO DA PRIMA
if test -z $1
then 	echo Errore: voglio almeno un parametro
	exit 1
fi
#in realta' il controllo precedente non e' un controllo stretto ma lasco e quindi in questo caso lo script accetta almeno un parametro, ma non da' errore nel caso di passaggio di piu' parametri e quindi se si vuole un controllo stretto si deve usare il controllo visto prima per if5.sh
#comunque se si arriva qui vuole dire che e' stato passato almeno un parametro ($1)
#controlliamo che il primo parametro sia un file e sia leggibile!
for parametri in $*
do
if test ! -f $parametri -o ! -r $parametri
then 	echo Errore: $parametri non file oppure non leggibile
	exit 2
fi
#se arriviamo qui vuol dire che $1 e' un file leggibile!
#chiediamo all'utente se vuole visualizzare il contenuto dei file:
#N.B. forziamo l'output sul terminale corrente per non avere problemi se si usasse la rdirezione dello standard output
echo "vuoi visualizzare il file $parametri (si/no)?" > /dev/tty
#leggiamo la risposta dell'utente
read var1
#se l'utente ha risposto 'si' allora procediamo con visualizzazione delle info e del file
if test $var1 = si
then
       	echo info del file $parametri
	ls -la $parametri	#potrebbe anche essere un nome che inizia con . e quindi usiamo anche -a
	echo contenuto del file $parametri
	cat $parametri
else 	echo niente stampa di $parametri
fi
done
