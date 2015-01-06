#!/bin/bash

echo ""
for f in ../input/000*.bin; do
    img=$(basename $f)
    diff -q ../input/$img ../output/$img >> /dev/null 2>> /dev/null
    res=$?
    if [ $res -eq 0 ]; then
    	echo "$img -> match"
    elif [ $res -eq 1 ]; then
    	echo "$img -> does not match"
    else
    	echo "$img -> missing"
    fi
done
