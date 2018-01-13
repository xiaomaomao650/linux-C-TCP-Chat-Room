#!/bin/bash

read -p "input num:" num

case $num in
	0)echo "00000";;
	1)echo "11111";;
	2|3|4) echo "223344";;
	*)echo "other";;
esac






