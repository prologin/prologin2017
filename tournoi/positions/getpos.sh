#!/bin/bash

for f in "$1/"*.html; do
	xmllint --html --xpath '//table[1]//td[4]' $f | sed 's#<td>\([^<]\+\)</td>#\1\n#g' | tail -n +2 > $( basename ${f%.html}.txt );
done

logins=$( sort *.txt | uniq )
echo '['
first1=1
for l in $logins; do
	if [ "$first1" -eq 0 ]; then
		echo ,
	fi
	first1=0
	echo -n '{"name": "'$l'", '
	echo -n '"visible": false,'
	echo -n '"data": ['
	first2=1
	for f in *.txt; do
		if [ "$first2" -eq 0 ]; then
			echo -n ,
		fi
		first2=0
		rank=$( grep -n $l $f | cut -d: -f1 )
		rank=${rank:-null}
		if [ -n "$rank" -a "$rank" != "null" ]; then
			rank=$(( $( wc -l $f | cut -d' ' -f1 ) - $rank ))
		fi
		echo -n $rank
	done
	echo -n ']}'
done
echo ']'
