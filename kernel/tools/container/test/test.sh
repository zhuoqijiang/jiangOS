#!/bin/bash


for arg in "$*"
    do 
        gcc -g ${arg}.c ../${arg}.c -o ${arg} 
    done



#valgrind --leak-check=yes ./list
