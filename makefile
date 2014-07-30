all:	seq_write_sync parser rand_write_sync

seq_write_sync:	seq_write_sync.c
	gcc -o seq_write_sync -lm -lpthread seq_write_sync.c -D_GNU_SOURCE 

parser:	parser.c
	gcc -o parser parser.c

rand_write_sync:	rand_write_sync.c
	gcc -o rand_write_sync -lm -lpthread rand_write_sync.c -D_GNU_SOURCE 
	
