#!/bin/bash

i=0
sum=0
while [ $i -le 100 ]
do
	let sum=$sum+$i	
	let i=$i+1
done
echo $sum

sum=0
i=100
until [ $i -le 0 ]
do
	let sum=$sum+$i	
	let i=$i-1
done
echo $sum
