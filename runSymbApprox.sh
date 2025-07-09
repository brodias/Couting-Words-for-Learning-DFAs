num_file=11
n=8
h=14
start_test=1
nb_test=10
echo ""
echo "Start approximation for file ${num_file} for n = ${n}"
for j in `seq $start_test $nb_test`
do
startDFA=Result/Result_Symbolic_Approximation/Summary${num_file}/DFAs/StartDFA_${num_file}_n_${n}_test_&{j}.dot
Trace=Traces/Traces${num_file}.words
DFA=Result/Result_Symbolic_Approximation/Summary${num_file}/DFAs/dfa${num_file}_n_${i}_test_${j}.dot
	timeout 1000 ./Symbolic_Approximation/main ${Trace} ${startDFA} ${n} ${h} 100 10 && ./Symbolic/infer --log=INFO -o "stats.json" specific DFA -f "${Trace}" --dfa="${DFA}" --dfa-starting="${startDFA}" -n=$i --method=SYMB 2> Result/Result_Symbolic/Summary${num_file}/DetailedSummary/Res_${num_file}_n_${i}_test_${j} && grep -Eo '[0-9\.]+s elapsed' Result/Result_Symbolic/Summary${num_file}/DetailedSummary/Res_${num_file}_n_${i}_test_${j} >> Result/Result_Symbolic/Summary${num_file}/TimeSummary/Time${num_file}_n_${i} || echo "Timeout: 1000s" >> Result/Result_Symbolic/Summary${num_file}/TimeSummary/Time${num_file}_n_${i}
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
