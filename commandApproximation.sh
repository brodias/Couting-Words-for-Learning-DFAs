#1: Trace
#2: startDFA
#3: n
#4: h
#5: InitRand
#6: NbRun
#7: ResApp
#8: DFA
#9: Res
./Symbolic_Approximation/main "${1}" "${2}" "${3}" "${4}" "${5}" "${6}" > ${7};
echo "Done with approximation";
./Symbolic/infer --log=INFO -o "stats.json" specific DFA -f ${1} --dfa=${8} --dfa-starting=${2} -n=${3} --method=SYMB 2> ${9}
