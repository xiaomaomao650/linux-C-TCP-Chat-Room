#!/bin/bash

echo $#
echo $0 $1 $2 $3

function test()
{
	echo $1 $2 $3
	return 123 
}

test aaa bbb cc

echo $?
