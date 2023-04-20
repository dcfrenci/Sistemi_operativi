#controllo il numero di parametri
case $# in
0) echo "Visualizzo gli elementi della e delle sottodirectory"
	ls -R | more 
	exit 1;;
1) echo "Numero corretto dei parametri";;
*) echo "Numero dei parametri troppo grande"
	exit 2;;
esac
#eseguo il file DIR.sh
if test -f $1
then	echo "E' un file"
	echo "Il file $1 contiene ls -l $1"
else 	echo "E' una directory"
	echo "La directory $1 contiene ls -ld $1"
fi
#echo SONO DIR.sh
#echo 'Il valore di $0 ===>' $0
#echo 'Il valore di $1 ===>' $1
#echo "DEBUG-Ora eseguo ls -l $1"
#ls -l $1
