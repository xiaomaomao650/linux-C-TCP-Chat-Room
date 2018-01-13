#!/bin/bash

#AAA=100  #定义变量  ---#注释
#str="hello world"

#read -p "input:" ptr
#echo $AAA
#echo $str   $ptr


#-----------数组------------------------------

read -p "input array:" -a arr   #输入一个数组
 
echo ${arr[*]}  #输出数组所有元素
echo ${arr[0]}  #输出对应下标元数
echo ${#arr[*]}  #输出数组元数个数

arr[1]=123
echo ${arr[*]}


#------------控制语句-------------------------
#if 语句
read -p "input num:" num

#数值测试
if [ $num -eq 100 ];then
	#-eq判断相等
	echo $num "与100相等"
fi

#数值测试---c语言格式实现
if ((num == 100))
then
	echo "相等"
fi


#问测试
read -p "input file path:"  fp
if [ -e $fp ];then
	echo "文件存在"
	
	tar -jcf my.tar.bz2 $fp

else
	echo "文件不存在"
fi



