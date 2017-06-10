/*
 * @author: Vishnu Bharadwaj
 *
 * args: parsed assembly file
 * containing 1s and 0s
 */

#include "func.h"

pthread_t PIPE1_THRD, PIPE2_THRD;

/* for pthread_cond_wait and pthread_cond_signal */
pthread_mutex_t PIPE1_THRD_MUTX;
pthread_cond_t PIPE1_THRD_COND;
pthread_mutex_t PIPE2_THRD_MUTX;
pthread_cond_t PIPE2_THRD_COND;

unsigned char FILEWRITE_FLAG = FALSE;
/*
 * method: proc_init()
 * function: initialise and create threads
 */
unsigned char process_init()
{

	char thrd_eval;
	printf("Initializing processor ...\n");
	printf("creating threads ...\n");

	thrd_eval = pthread_create(&PIPE1_THRD, NULL, (void *) &pipe1_thrd, NULL);
	if( thrd_eval!= 0)
	{
		error_print("thread creation failed for PIPE1_THRD eval: %d",thrd_eval);
		exit(0);
	}

	thrd_eval = pthread_create(&PIPE2_THRD, NULL, (void *) &pipe2_thrd, NULL);
	if( thrd_eval!= 0)
	{
		error_print("thread creation failed for PIPE2_THRD eval: %d",thrd_eval);
		exit(0);
	}

	return TRUE;
}


/*
 * method : main
 */
int main(int argc, char *argv[])
{
	FILE *asmParsed_file;
	char *file_char = NULL;
	size_t fline_n = 2;

	FILE *ddpout_file;
	char ddpoutfile_name[] = "ddpout_";

	printf("\n<<<<<< Dual Decode Parallel processing >>>>>>\n\n");

	int core_id = 1;
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
	CPU_SET(core_id, &cpuset);
	pthread_t current_thread = pthread_self();
	pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset);

	if(process_init() != 1)
	{
		debug_print("processor initialization failed\n",0);
//		exit(0);
	}

	if(argc != 2)
	{
		printf(" Please enter one instruction file\n",0);
	}
	else
	{
		printf("processing file '%s' ...\n",argv[1]);
	}

	strcat(ddpoutfile_name,argv[1]); /* writing to file with appropriate name */
	ddpout_file = fopen(ddpoutfile_name, "w");

/* read the assembly parsed file */
	asmParsed_file = fopen(argv[1],"r");
	while( (getline(&file_char,&fline_n,asmParsed_file)) > 0 ) /* 1 and 0 is generally given by compiler reordering or optimizations */
	{
		pthread_cond_signal(&PIPE1_THRD_COND);
		/* use atoi(file_char) to convert char to int from the file read */
		if(atoi(file_char) == 1){
			fprintf(ddpout_file,"%c\n",'1'); /* writing o/p to file */
			pthread_cond_signal(&PIPE2_THRD_COND); /* iff file_char is 1*/
		}
		else
		{
			fprintf(ddpout_file,"%c\n",'0');
		}
	}
	fclose(asmParsed_file);
	fclose(ddpout_file);

	pthread_join(PIPE1_THRD,NULL);
	pthread_join(PIPE2_THRD,NULL);

	return TRUE;
}
