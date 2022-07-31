#!/bin/bash


for arg in "$*"
	do 
		gcc ${arg}.c ../${arg}.c -o ${arg} 
	done
