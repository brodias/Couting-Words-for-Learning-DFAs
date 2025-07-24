start_file=12
end_file=12
min_n=1
max_n=8
start_test=9
nb_test=10

echo "Summary $1 computations:"
for num_file in $2 #02 03 04 05 06 07 08 09 10 11 12 13 14 15
do
	File=Summary.py
	echo "import numpy as np" > $File
	echo "" >> $File
	echo "L_SYMB_${num_file} = []" >> $File
	echo "File ${num_file}:"
	computation_done=false
	for i in `seq $min_n $max_n`
	do
		TimeSummary=Result/Result_$1/Summary${num_file}/TimeSummary/Time_${num_file}_n_${i}
		if test -f $TimeSummary
		then
			computation_done=true
			echo -n "L_SYMB_${num_file}_n_${i} = [" >> $File
			count=$( grep -c -Eo '[0-9\.]+s e' $TimeSummary)
			countTimeout=$( grep -c -Eo 'Timeout' $TimeSummary)
			for j in `seq 1 $count`
			do
				grep -Eo '[0-9\.]+s e' $TimeSummary | head -$j | tail -1 | echo -n $(grep -Eo '[0-9\.]+') >> $File
				if [ $j = $count ] && [ $countTimeout = 0 ]
				then :
				else echo -n "," >> $File
				fi
			done
			for j in `seq 1 $countTimeout`
			do
				echo -n '1000' >> $File
				if [[ $j = $countTimeout ]]
				then :
				else echo -n "," >> $File
				fi
			done
			echo "]" >> $File
			echo "" >> $File
			echo "L_SYMB_${num_file}.append(($i,L_SYMB_${num_file}_n_${i}))" >> $File
			#echo "Done with n = ${i}"
		fi
	done
	if [ "$computation_done" = false ]
	then echo "No compuation done with this file"
	fi
	#echo "Done!"
	#echo ""
	echo "L_SYMB_${num_file}_res = [(L_SYMB_${num_file}[i][0],len(L_SYMB_${num_file}[i][1]),np.mean(L_SYMB_${num_file}[i][1]),np.std(L_SYMB_${num_file}[i][1])) for i in range(len(L_SYMB_${num_file}))]" >> $File
	echo "" >> $File
	echo "for x in L_SYMB_${num_file}_res:" >> $File
	#echo -e -n ' \t ' >> $File
	echo "    i = x[0]" >> $File
	echo "    l = x[1]" >> $File
	echo "    mean = round(x[2],3)" >> $File
	echo "    var = round(x[3],3)" >> $File
	echo "    print(str(i),':','mean:',mean,', var:',var,', nb_exp:',l)"  >> $File
	python3 $File
	echo ""
done
