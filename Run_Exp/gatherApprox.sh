min_n=1
max_n=8

for num_file in $2
do
	File=Run_Exp/Summary.py
	echo "import numpy as np" > $File
	echo "" >> $File
	echo "Start File ${num_file}"
	for i in `seq $min_n $max_n`
	do
		TimeSummary=Result/Result_$1_Approximation/Summary${num_file}/TimeSummary/Time_${num_file}_n_${i}
		if test -f $TimeSummary
		then
			echo -n "L_SYMB_${num_file}_n_${i} = [" >> $File
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
			echo "l = len(L_SYMB_${num_file}_n_${i})" >> $File
			echo "mean = round(np.mean(L_SYMB_${num_file}_n_${i}),3)" >> $File
			echo "var = round(np.std(L_SYMB_${num_file}_n_${i}),3)" >> $File
			echo "print(str($i),':','mean:',mean,', var:',var,', nb_exp:',l)"  >> $File
			echo "Done with n = ${i}"
		fi
	done
	echo "Done!"
	echo ""
	python3 $File
done
