#!/bin/bash
for i in 0 1 2 3 4 5 6 7
do
	dd if=/dev/zero of=test$i.img bs=8M count=1024
done		
