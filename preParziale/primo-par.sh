#riporta solo il primo parametro
val=0
for elem in $*
	do
		if test $val -eq 0
			then 	echo Il primo parametro è $elem
				val=1
				echo Lista privata del primo elemento
			else	echo $elem
		fi
	done

