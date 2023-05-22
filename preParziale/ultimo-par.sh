#riporto l'ultimo elemento della lista
echo L'ultimo elemento della lista è $#
#riporto la lista privata dell'ultimo elemento
eval `echo $`$#``
val=1
for elem in $*
	do
		if test $val -ne $#
			then	echo $elem
				val=`expr $val + 1`
		fi
	done
