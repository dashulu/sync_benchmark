#!/bin/bash
for i in 1 2 4 6 8
do
	for j in 4 128 256 512 1024 2048 4096 8192 
	do
		num3=`echo "sclae=2; $j/4" | bc`
		echo $num3
		echo "rand_write_sync thread " $i "bs " $j
		./rand_write_sync $j 1 $num3 $i
		sleep 1
	done
done		
