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
		timeout 1000 ./Symbolic/infer --log=INFO -o "stats.json" specific DFA -f "Traces/Traces${num_file}.words" --dfa="Result/Result_Symbolic/Summary${num_file}/DFAs/dfa${num_file}_n_${i}_test_${j}.dot" -n=$i --method=SYMB 2> Result/Result_Symbolic/Summary${num_file}/DetailedSummary/Res_${num_file}_n_${i}_test_${j} && grep -Eo '[0-9\.]+s elapsed' Result/Result_Symbolic/Summary${num_file}/DetailedSummary/Res_${num_file}_n_${i}_test_${j} >> Result/Result_Symbolic/Summary${num_file}/TimeSummary/Time${num_file}_n_${i} || echo "Timeout: 1000s" >> Result/Result_Symbolic/Summary${num_file}/TimeSummary/Time${num_file}_n_${i}
		if grep -q -Eo 'DFA found in: [0-9\.]+s' Result/Result_Symbolic/Summary${num_file}/DetailedSummary/Res_${num_file}_n_${i}_test_${j} 
		then 
		grep -Eo 'DFA found in: [0-9\.]+s' Result/Result_Symbolic/Summary${num_file}/DetailedSummary/Res_${num_file}_n_${i}_test_${j} | tail -1 >> Result/Result_Symbolic/Summary${num_file}/TimeSummary/Time${num_file}_n_${i}
		else echo "Only one DFA attempted" >> Result/Result_Symbolic/Summary${num_file}/TimeSummary/Time${num_file}_n_${i}
		fi
		echo "" >> Result/Result_Symbolic/Summary${num_file}/TimeSummary/Time${num_file}_n_${i}
		echo ""
		echo "Done with test = ${j}"
	done
	echo ""
	echo "Done with n = ${i}"
done
done
