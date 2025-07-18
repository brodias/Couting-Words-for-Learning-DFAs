#start_file=3
#end_file=3
#`seq $start_file $end_file`

#min_n=5
#max_n=5
#for num_file in 01 04 07 10 11 13
#start_test=1
#nb_test=10

#min_n=4
#max_n=4
#start_test=2
#nb_test=10
#for num_file in 06

#min_n=5
#max_n=5
#start_test=2
#nb_test=10
#for num_file in 09

#min_n=4
#max_n=4
#start_test=3
#nb_test=10
#for num_file in 12

min_n=1
max_n=4
start_test=1
nb_test=10
for num_file in 15
do
echo ""
echo "Start File ${num_file}"
for i in `seq $min_n $max_n`
do
	h=$(($i + $i-2))
	echo ""
	echo "Start: n = ${i}"
	for j in `seq $start_test $nb_test`
	do
		echo ""
		echo "Start test = ${j}"
		Trace=Traces/Traces${num_file}.words
		Res=Result/Result_ILP/Summary${num_file}/DetailedSummary/Res_${num_file}_n_${i}_test_${j}
		DFA=Result/Result_ILP/Summary${num_file}/DFAs/Count_dfa${num_file}_n_${i}_test_${j}.dot
		TimeSummary=Result/Result_ILP/Summary${num_file}/TimeSummary/Time_${num_file}_n_${i}
		timeout 1000 python3 ILP/Main.py $Trace $DFA $i $h 1000 > $Res || echo "Timeout: 1000s" >> $TimeSummary
		grep -Eo '[0-9\.]+s elapsed' $Res >> $TimeSummary 
		if grep -q -s -Eo '[0-9\.]+s' $Res | tail -2
		then 
		echo -n "Time for solving after building the model:" >> $TimeSummary
		grep -Eo ' [0-9\.]+s' $Res | tail -1 >> $TimeSummary 
		fi
		echo "" >> $TimeSummary
		echo "Done with test = ${j}"
	done
	echo ""
	echo "Done with n = ${i}"
done
done
