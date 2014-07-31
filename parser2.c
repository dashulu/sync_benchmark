#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

void main(int argc, char** argv) {
	int bs;
	int num_threads;
	char filename[80];
	char tmp[20];
	int length;
	int i;
	
	if(argc != 3) {
		printf("arg error!\n");
		exit(1);
	}
	
	bs = atoi(argv[1]);
	num_threads = atoi(argv[2]);

	filename[0] = 'r';
	filename[1] = 'a';
	filename[2] = 'n';
	filename[3] = 'd';
	filename[4] = 'o';
	filename[5] = 'm';
	filename[6] = '_';
	filename[7] = '\0';
	strcat(filename, argv[1]);
	strcat(filename, "_1_");
	sprintf(tmp, "%d_", bs/4);
	strcat(filename, tmp);
	strcat(filename, argv[2]);

	length = strlen(filename);

	FILE * f;
	int time;
	int count = 0;
	uint64_t total = 0;
		
	for(i = 0;i < num_threads;i++) {
		filename[length] = '_';
		filename[length + 1] = i + 48;
		filename[length + 2] = '\0';

		f = fopen(filename, "r");
		if(f == NULL) {
			printf("open file error.file name:%s\n", argv[1]);
			exit(1);
		}

		while(fscanf(f, "%d", &time) != EOF) {
			total += time;
			count++;
		}
		fclose(f);
	}	
	printf("thread's number:%d total:%lu  count:%d avg:%lu\n", num_threads, total, count, total/count);
}
