#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"

void main(int argc, char** argv) {
	if(argc != 2) {
		printf("arg error!\n");
		exit(1);
	}
	
	FILE * f;
	f = fopen(argv[1], "r");
	if(f == NULL) {
		printf("open file error.file name:%s\n", argv[1]);
		exit(1);
	}

	int time;
	int count = 0;
	uint64_t total = 0;
	while(fscanf(f, "%d", &time) != EOF) {
		total += time;
		count++;
	}
	printf("total:%lu  count:%d avg:%lu\n", total, count, total/count);
	fclose(f);
}
