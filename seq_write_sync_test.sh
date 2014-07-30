#!/bin/bash
for i in 8
do
	for j in 512 1024 2048 4096 8192 
	do
		num3=`echo "sclae=2; $j/4" | bc`
		echo $num3
		echo "seq_write_sync thread " $i "bs " $j
		./seq_write_sync $j 1 $num3 $i
		sleep 1
	done
done		
