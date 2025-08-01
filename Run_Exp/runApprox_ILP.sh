num_file=21
n=8
h=7
time=250
start_test=10
nb_test=10
echo ""
echo "Start ILP approximation for file ${num_file} for n = ${n}"

for j in `seq $start_test $nb_test`
do
	startDFA=Result/Result_ILP_Approximation/Summary${num_file}/DFAs/Start_dfa_${num_file}_n_${n}_test_${j}.dot
	Trace=Traces/Traces${num_file}.words
	DFA=Result/Result_ILP_Approximation/Summary${num_file}/DFAs/dfa${num_file}_n_${n}_test_${j}.dot
	ResApp=Result/Result_ILP_Approximation/Summary${num_file}/DetailedSummary/ResApprox_${num_file}_n_${n}_test_${j}
	Res=Result/Result_ILP_Approximation/Summary${num_file}/DetailedSummary/Res_${num_file}_n_${n}_test_${j}
	TimeSummary=Result/Result_ILP_Approximation/Summary${num_file}/TimeSummary/Time_${num_file}_n_${n}

	timeout 1000 ./Run_Exp/approx_ILP.sh "$Trace" "$startDFA" "$n" "$h" "$time" "$ResApp" "$DFA" "$Res";
	if grep -q -s -Eo '[0-9\.]+s elapsed' ${Res} 
	then 
		# There was no timeout
		# First, approximation
		if grep -s -q -Eo '[0-9\.]+s elapsed' ${ResApp} 
		then 
			echo -n "For approximation: " >> ${TimeSummary}
			grep -Eo '[0-9\.]+s elapsed' ${ResApp} >> ${TimeSummary}
		else :
		fi
		# Then, SAT solving 
		echo -n "For solving from starting DFA: " >> ${TimeSummary}
		grep -Eo '[0-9\.]+s elapsed' ${Res} >> ${TimeSummary}
		# Last DFA found (no timeout, so some DFA worked).
		if grep -q -Eo 'DFA found in: [0-9\.]+s' ${Res} 
		then 
			grep -Eo 'DFA found in: [0-9\.]+s' ${Res} | tail -1 >> ${TimeSummary}
		else 
			echo "The starting DFA worked!" >> ${TimeSummary}
		fi
	else 
		# There was a timeout
		echo "Timeout: 1000s" >> ${TimeSummary}
		if grep -q -s -Eo '[0-9\.]+s elapsed' ${ResApp}
		then 
			echo -n "For approximation: " >> ${TimeSummary}
			grep -Eo '[0-9\.]+s elapsed' ${ResApp} >> ${TimeSummary}
			grep -Eo 'DFA found in: [0-9\.]+s' ${Res} | tail -1 >> ${TimeSummary}
		else :
		fi
	fi	
	echo "" >> ${TimeSummary}
	echo ""
	echo "Done with test = ${j}"
done
echo ""
echo "Done with n = ${n}"
