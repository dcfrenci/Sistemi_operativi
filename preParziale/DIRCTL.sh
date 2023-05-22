#controllo il numero di parametri
case $# in
0) echo "Numero dei parametri insufficiente"
	exit 1;;
1) echo "Numero corretto dei parametri";;
*) echo "Numero dei parametri troppo grande"
	exit 2;;
esac
#eseguo il file DIR.sh
cd /home/dc/SOeLab22-23/Lezioni/SHELL
sh DIR.sh $1
#!/bin/sh
echo SONO DIR.sh
echo 'Il valore di $0 ===>' $0
echo 'Il valore di $1 ===>' $1
echo "DEBUG-Ora eseguo ls -l $1"
ls -l $1
