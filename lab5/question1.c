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

#define MAX_THREADS 10
#define DATA_SIZE 1000

int is_prime(int number) {
	if(number <= 1) return 0;
	else if(number == 2) return 1;

	if(number % 2 == 0 || number % 3 == 0) return 0;

	int flag = 1;

	for(int i = 5; i * i < number; i = i + 6) {
		if(number % i == 0 || number % (i + 2) == 0) { flag = 0; break; }
	}

	return flag;
}

void* prime(void *params) {
	int ints[DATA_SIZE];
	int prime_count = 0;

	for(int i = 0; i < sizeof(ints); i++) {
		int number = (rand() % (10000 - 1001) + 1000);
		ints[i] = number;
		if(is_prime(number)) {
			prime_count++;
		}
	}
	int threadIdx = *((int*) params);


	printf("Thread %d - %d primes\n", threadIdx, prime_count);
}

int main( void ) {
	pthread_t threadIds[MAX_THREADS];
	int threadIdx[MAX_THREADS];

	for(int i = 0; i < MAX_THREADS; i++) {
		threadIdx[i]=i;
		if(pthread_create(&threadIds[i], NULL, prime, &threadIdx[i]) != 0) {
			perror("thread create");
			exit(1);
		}
	}

	for(int i = 0; i < MAX_THREADS; i++) {
		if(pthread_join(threadIds[i], NULL) != 0) {
			perror("thread join");
			exit(1);
		}
	 }
}



