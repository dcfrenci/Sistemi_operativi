for elem in `ls $1`
	do
		if test -f $elem
			then mv $elem Esercizi
		fi
	done

