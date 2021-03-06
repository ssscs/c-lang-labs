/**
 * Copyright 2021 Sinan Can Soysal
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); 
 * you may not use this file except in compliance with the License. 
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, 
 * software distributed under the License is distributed on an "AS IS" BASIS, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
 * See the License for the specific language governing permissions and limitations under the License.
**/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

#define MAX_THREADS 25

int *args;

int isDivisible(int num, int divider) {
	if(num % divider == 0) return 1;
	return 0;
}

void writeToFile(int *ints, char *filename) {
	FILE *file = fopen(filename, "w");

	for(int i = 0; i < sizeof(ints); i++) fprintf(file, "%d ", ints[i]);

	fclose(file);
}

void* compute_d(void* params) {
	int  thread_idx = *((int*) params);
	int *d_count = (int*)malloc(sizeof(int));
	*d_count = 0;

	// filename = "numbers<thread_id>.txt"
	char filename[50];
	sprintf(filename, "numbers%d.txt", thread_idx);

	FILE *file = fopen(filename, "w");

	for(int i = 0; i < args[4]; i++) {
		int num = (rand() % (args[1] - (args[0] + 1))) + args[0];
		if(isDivisible(num, args[2]) && !(isDivisible(num, args[3]))) (*d_count)++;
		fprintf(file, "%d ", num);
	}

	fclose(file);
	return d_count;
}

int main(int argc, char *argv[]) {
	// handling passed arguments
	if(argc != 6) { printf("Unexpected numbers arguments.\nTerminating..\n"); exit(1); }

	args = (int*) malloc((argc - 1) * sizeof(int));

	for(int i = 1; i < argc; i++) args[i-1] = atoi(argv[i]);

	if(args[0] > args[1]) {
		int temp = args[0];
		args[0] = args[1];
		args[1] = temp;
	} else if (args[0] == args[1]) {
		printf("I can't create integers between same numbers: %d == %d\nTerminating..", args[0], args[1]);
		exit(1);
	}

	// setting up and starting threads
	pthread_t thread_ids[MAX_THREADS];
	int thread_idx[MAX_THREADS];
	int *results[MAX_THREADS];

	for(int i = 0; i < MAX_THREADS; i++) {
		thread_idx[i] = i;
		if(pthread_create(&thread_ids[i], NULL, compute_d, &thread_idx[i]) != 0) {
			perror("error in thread creation");
                        exit(1);
                }
	}

	// joining threads
	for(int i = 0; i < MAX_THREADS; i++) {
		if(pthread_join(thread_ids[i], (void *)&results[i]) != 0) {
			perror("error in thread join");
                        exit(1);
                }
	}

	// print results
	for(int i = 0; i < MAX_THREADS; i++) {
		printf("Thread %d - %d\n", i, *results[i]);
	}
	free(args);
}
