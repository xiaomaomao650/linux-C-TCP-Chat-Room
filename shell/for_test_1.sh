#!/bin/bash


sum_o=0
sum_v=0

oi=0
vi=0

oarr=0
varr=0

for i in `seq 0 100`
do 
	let t=$i%2
	if [ $t -eq 0 ];then
		let sum_o=$sum_o+$i
		((oarr[oi] = i))
		((oi++))
	else
		let sum_v=$sum_v+$i
		((varr[vi] = i))
		((vi++))
	fi
done

echo $sum_o  $sum_v
echo ${oarr[*]}
echo ${varr[*]}
