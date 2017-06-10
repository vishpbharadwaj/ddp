/*
 * func.c
 *
 *  Created on: 02-Jun-2017
 *      Author: vishnu
 */

#include "func.h"

extern pthread_t PIPE1_THRD, PIPE2_THRD;
/* for pthread_cond_wait and pthread_cond_signal */
extern pthread_mutex_t PIPE1_THRD_MUTX;
extern pthread_cond_t PIPE1_THRD_COND;
extern pthread_mutex_t PIPE2_THRD_MUTX;
extern pthread_cond_t PIPE2_THRD_COND;

/*
 * method: pipe1_thrd
 * function: simulate pipeline 1
 * containing 5 stages
 */
void pipe1_thrd()
{
	int core_id = 2;
	cpu_set_t cpuset;
	CPU_SET(core_id, &cpuset);

    unsigned long mask = 1; /* processor 3 */
    /* bind process to processor 3 */
    if (pthread_setaffinity_np(PIPE1_THRD, sizeof(cpu_set_t), &cpuset) < 0) {
        perror("pthread_setaffinity_np");
    }

	while(1){
		pthread_cond_wait(&PIPE1_THRD_COND, &PIPE1_THRD_MUTX);
		printf("processing pipe 1\n");
#ifdef PIPE_PRINTS
		printf("fetching in 1\n");
		printf("decoding in 1\n");
		printf("executing in 1\n");
		printf("memory op in 1\n");
		printf("writing back in 1\n");
#endif
	}
}

/*
 * method: pipe2_thrd
 * function: simulate pipeline 2
 * containing fetch & decode block
 */
void pipe2_thrd()
{
	int core_id = 2;
	cpu_set_t cpuset;
	CPU_SET(core_id, &cpuset);

    unsigned long mask = 4; /* processor 4 */
    /* bind process to processor 4 */
    if (pthread_setaffinity_np(PIPE2_THRD, sizeof(cpu_set_t), &cpuset) < 0) {
        perror("pthread_setaffinity_np");
    }

	while(1){
		pthread_cond_wait(&PIPE2_THRD_COND, &PIPE2_THRD_MUTX);
		printf("processing pipe 2\n");
#ifdef PIPE_PRINTS
		printf("fetching in 2\n");
		printf("decoding in 2\n");
#endif
	}
}
