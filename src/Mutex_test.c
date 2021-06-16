#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int data = 0;

pthread_mutex_t mutex;

void* threadFunc1(char *str){
	pthread_mutex_lock(&mutex);
	if (strlen(str)+strlen("hello") >= 100)
	{
		str = realloc(str, 200);
	}
	strncpy(str+strlen(str),"hello",strlen("hello")+1);
	pthread_mutex_unlock(&mutex);
	return str;
	pthread_exit(0);
}

void* threadFunc2(char *str){
	pthread_mutex_lock(&mutex);
	if (strlen(str)+strlen("15") >= 100)
	{
		str = realloc(str, 200);
	}
	strncpy(&str[strlen(str)],"15",strlen("15")+1);
	pthread_mutex_unlock(&mutex);
	pthread_exit(0);
}

int main(void) {
	int i = 0;
	int result = 0;
	pthread_t thread1;
	pthread_t thread2;
	char *str = (char*)malloc(100);
	memset(str, 0, 100);

	// mutex initialization
	pthread_mutex_init(&mutex, NULL);
	for(i = 0; i < 5; i++)
	{
		result = pthread_create(&thread1, NULL, (void*)threadFunc1, str);
		if ( result != 0)
		{
			printf("Error create thread1\n");
		}
		result = pthread_create(&thread2, NULL, (void*)threadFunc2, str);
		if ( result != 0)
		{
			printf("Error create thread2\n");
		}
		pthread_join(thread2, NULL);
		pthread_join(thread1, NULL);
	}
	// destroy mutex
	pthread_mutex_destroy(&mutex);
	printf("%s\n",str);
	return EXIT_SUCCESS;
}
