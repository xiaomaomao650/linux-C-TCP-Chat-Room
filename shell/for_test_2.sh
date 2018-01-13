#!/bin/bash

read -p "input dir path:" dpath

for i in `ls $dpath`
do
	if [ -d "$dpath/$i" ];then
		echo "$i is dir"  #"里面的变量是可以输出"  ‘里面的变量只输出名字’
	else
		echo "$i is file"
	fi
done
