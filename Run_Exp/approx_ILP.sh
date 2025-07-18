#1: Trace
#2: startDFA
#3: n
#4: h
#5: time
#6: ResApp
#7: DFA
#8: Res
python3 ./ILP/Main.py "${1}" "${2}" "${3}" "${4}" "${5}" > ${6};
echo "Done with approximation";
./Symbolic/infer --log=INFO -o "stats.json" specific DFA -f ${1} --dfa=${7} --dfa-starting=${2} -n=${3} --method=SYMB 2> ${8}
