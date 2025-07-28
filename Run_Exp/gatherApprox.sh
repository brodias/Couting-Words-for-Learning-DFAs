min_n=1
max_n=8

echo "Summary $1 approximations:"
echo ""
for num_file in $2
do
	File=Run_Exp/Summary.py
	echo "import numpy as np" > $File
	echo "" >> $File
	echo "File ${num_file}:"
	approx_done=false
	for i in `seq $min_n $max_n`
	do
		TimeSummary=Result/Result_$1_Approximation/Summary${num_file}/TimeSummary/Time_${num_file}_n_${i}
		if test -f $TimeSummary
		then
			approx_done=true
			echo -n "L_SYMB_${num_file}_n_${i} = [" >> $File
			countStart=$( grep -c -Eo 'The starting DFA worked!' $TimeSummary)
			countNotStart=$( grep -c -Eo 'DFA found in:' $TimeSummary)
			tout=$( grep -c -Eo 'Timeout' $TimeSummary)
			count=$( grep -c -Eo 'Timeout: 1000| DFA: [0-9\.]+' $TimeSummary)
			for j in `seq 1 $count`
			do
				echo -n "min(1000," >> $File
				grep -Eo 'Timeout: 1000| DFA: [0-9\.]+' $TimeSummary | head -$j | tail -1 | echo -n $(grep -Eo '[0-9\.]+') >> $File
				echo -n "+" >> $File
				grep -Eo 'tion: [0-9\.]+' $TimeSummary | head -$j | tail -1 | echo -n $(grep -Eo '[0-9\.]+') >> $File
				echo -n ")" >> $File
				if [ $j = $count ]
				then :
				else echo -n "," >> $File
				fi
			done
			echo "]" >> $File
			echo "start = $countStart" >> $File
			echo "not_start = $countNotStart" >> $File
			echo "timeout = $tout" >> $File
			echo "l = len(L_SYMB_${num_file}_n_${i})" >> $File
			echo "mean = round(np.mean(L_SYMB_${num_file}_n_${i}),3)" >> $File
			echo "var = round(np.std(L_SYMB_${num_file}_n_${i}),3)" >> $File
			echo "print(str($i),':','mean:',mean,', var:',var,', nb_exp:',l,', timeout:',timeout,', start_ok:',start,', start_not_ok:',not_start)"  >> $File
			#echo "Done with n = ${i}"
		fi
	done
	if [ "$approx_done" = false ]
	then echo "No approximation done with this file"
	fi
	#echo "Done!"
	python3 $File
	echo ""
done
