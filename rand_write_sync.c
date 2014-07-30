#include <pthread.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>


int every[8][8000000];
int every_count[8];
int sync_freq;
uint64_t file_size;
int bs_size;
int num_threads;

int my_rand() {
	int ret;
	ret = rand();
	ret = ret - (ret % 4096);
	return ret;
}

void write_result(int index) {
	char filename[80];
	char tmp[20];
	filename[0] = 'r';
	filename[1] = 'a';
	filename[2] = 'n';
	filename[3] = 'd';
	filename[4] = 'o';
	filename[5] = 'm';
	filename[6] = '_';
	filename[7] = '\0';
	sprintf(tmp, "%d_", bs_size);
	strcat(filename, tmp);
	sprintf(tmp, "%d_", sync_freq);
	strcat(filename, tmp);
	sprintf(tmp, "%d_", file_size/1024);
	strcat(filename, tmp);
	sprintf(tmp, "%d_", num_threads);
	strcat(filename, tmp);
	sprintf(tmp, "%d", index);
	strcat(filename, tmp);

	FILE* fp = fopen(filename, "w");
	int i;

	for(i = 0;i < every_count[index];i++) {
		fprintf(fp, "%d\n", every[index][i]);
	}
	fdatasync(fp);
	close(fp);
}



void evaluation(int index) {
	char* buf;
	char filename[20];
	uint64_t count = 0;
	int i;
	int min;
	int max;
	uint64_t total;
	int avg;

	filename[0] = 't';
	filename[1] = 'e';
	filename[2] = 's';
	filename[3] = 't';
	filename[4] = index + 48;
	filename[5] = '.';
	filename[6] = 'i';
	filename[7] = 'm';
	filename[8] = 'g';
	filename[9] = '\0';
	int fp = open(filename, O_WRONLY | O_DIRECT, S_IRWXU | S_IROTH |S_IWOTH | S_IRWXG);
	
	if(fp <= 0) {
		printf("error open file:%s.\n", filename);
		return;
	}

	posix_memalign((void**)&buf, 4096, bs_size*1024);

	if(buf == NULL) {
		printf("thread%d failed\n",index);
		return;
	}	
	for(i = 0;i < 1024;i++) {
		buf[i] = 'a';
	}

	int k;
	struct timeval start, end;
	while(count < file_size) {
		for(i = 0;i < sync_freq;i++) {
			k = my_rand();
			lseek(fp, k, SEEK_SET);
			write(fp, buf, bs_size*1024);
			count += bs_size;
		}
		gettimeofday(&start, NULL);
		fsync(fp);
		gettimeofday(&end, NULL);
		every[index][every_count[index]] = (end.tv_sec - start.tv_sec)*1000000 + end.tv_usec - start.tv_usec;
		every_count[index]++;
	}
	free(buf);
	close(fp);
	min = 10000000;
	max = 0;
	total = 0;
	for(i = 0;i < every_count[index];i++) {
		if(min > every[index][i])
			min = every[index][i];
		if(max < every[index][i])
			max = every[index][i];
		total += every[index][i]; 
	}
	avg = total/every_count[index];
	total = 0;
	count = 0;
	for(i = 0;i < every_count[index];i++) {
		total += pow((every[index][i] - avg), 2);
		if(every[index][i] < avg*1.1 && every[index][i] > 0.9*avg)
			count++;
	}

	total = sqrt(total);


	printf("thread%d result:\n block size:%dk, file size:%dm, sync frequency:%d\n max sync time:%d min sync time:%d avg sync time:%d fang cha:%d zhixin:%f\n",
		index, bs_size, file_size, sync_freq, max, min, avg, total, count*1.0/every_count[index]);

}

int main(int argc, char** argv) {
	int i;
	int ret;
	pthread_t threads[8];

	if(argc != 5) {
		printf("parameter error! please input bs(k), sync frequency, size(m).\n");
		return 0;
	}
	sync_freq = atoi(argv[2]);
	file_size = atoi(argv[3]);
	bs_size = atoi(argv[1]);
	file_size *= 1024;
	num_threads = atoi(argv[4]);
	if(num_threads > 8)
		num_threads = 8;

	for(i = 0;i < num_threads;i++) {
		ret = pthread_create(&threads[i], NULL, (void*) &evaluation, i);
		if(ret != 0) {
			printf("线程%d创建失败\n", i);
		} else {
			printf("线程%d创建成功\n", i);
		}
	}
	for(i = 0;i < num_threads;i++) {
		pthread_join(threads[i], NULL);	
	}
	for(i = 0;i < num_threads;i++) {
		write_result(i);
	}
	return 1;	
}
