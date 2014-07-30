#!/bin/bash
for i in 1 2 4 6 8
do
	for j in 4 128 256 512 1024 2048 4096 8192 
	do
		for k in $(seq 0 $i)
		do
			num3=`echo "sclae=2; $j/4" | bc`
			echo "result_"$j"_1_"$num3"_"$i"_"$k
			./parser "result_"$j"_1_"$num3"_"$i"_"$k
			sleep 1
		done
	done
done		
