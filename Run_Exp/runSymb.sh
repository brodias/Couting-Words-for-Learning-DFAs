start_file=12
end_file=12
min_n=8
max_n=8
start_test=9
nb_test=10

for num_file in `seq $start_file $end_file`
do
echo ""
echo "Start File ${num_file}"
for i in `seq $min_n $max_n`
do
	echo ""
	echo "Start: n = ${i}"
	for j in `seq $start_test $nb_test`
	do
		Trace=Traces/Traces${num_file}.words
		DFA=Result/Result_Symbolic/Summary${num_file}/DFAs/dfa${num_file}_n_${i}_test_${j}.dot
		Res=Result/Result_Symbolic/Summary${num_file}/DetailedSummary/Res_${num_file}_n_${i}_test_${j}
		TimeSummary=Result/Result_Symbolic/Summary${num_file}/TimeSummary/Time_${num_file}_n_${i}
	
		timeout 1000 ./Symbolic/infer --log=INFO -o "stats.json" specific DFA -f "${Trace}" --dfa="${DFA}" -n=$i --method=SYMB 2> $Res || echo "Timeout: 1000s" >> $TimeSummary
		grep -Eo '[0-9\.]+s elapsed' Res >> $TimeSummary
		if grep -q -Eo 'DFA found in: [0-9\.]+s' $Res 
		then 
		grep -Eo 'DFA found in: [0-9\.]+s' $Res | tail -1 >> $TimeSummary
		else echo "Only one DFA attempted" >> $TimeSummary
		fi
		echo "" >> $TimeSummary
		echo ""
		echo "Done with test = ${j}"
	done
	echo ""
	echo "Done with n = ${i}"
done
done
