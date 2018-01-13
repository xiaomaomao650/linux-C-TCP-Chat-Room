#!/bin/bash

read -p "input arr:" -a arr

for i in ${arr[*]}
do
	echo $i	
done

sum=0
for i in `seq 0 100`
do 
	let sum=$sum+$i  #数值运算((sum=sum+i))
	#echo $i
done
echo $sum

for i in `ls`
do 
	echo $i
done
