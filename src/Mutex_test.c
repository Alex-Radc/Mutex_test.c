#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define DELTA 100

int data = 0;

pthread_mutex_t mutex;

typedef struct _BUFFER
{
	char *buff;
	int size_buff;
} BUFFER;

void* threadFunc1(BUFFER *buffer)
{
	pthread_mutex_lock(&mutex);

	if (strlen(buffer->buff) + strlen("hello") >= buffer->size_buff)
	{
		buffer->size_buff += DELTA;
		buffer->buff = realloc(buffer->buff, buffer->size_buff);
	}
	strncpy(buffer->buff + strlen(buffer->buff), "hello", strlen("hello") + 1);
	if ((strlen(buffer->buff) + 3 * DELTA) < buffer->size_buff)
	{
		buffer->size_buff -= DELTA * 2;
		buffer->buff = realloc(buffer->buff, buffer->size_buff);
	}
	pthread_mutex_unlock(&mutex);
	return buffer->buff;
	pthread_exit(0);
}

void* threadFunc2(BUFFER *buffer)
{
	pthread_mutex_lock(&mutex);
	if (strlen(buffer->buff) + strlen("15") >= buffer->size_buff)
	{
		buffer->size_buff += DELTA;
		buffer->buff = realloc(buffer->buff, buffer->size_buff);
	}
	strncpy(buffer->buff + strlen(buffer->buff), "hello", strlen("hello") + 1);
	if ((strlen(buffer->buff) + 3 * DELTA) < buffer->size_buff)
	{
		buffer->size_buff -= DELTA * 2;
		buffer->buff = realloc(buffer->buff, buffer->size_buff);
	}
	pthread_mutex_unlock(&mutex);
	return buffer->buff;
	pthread_exit(0);
}

int main(void)
{
	int i = 0;
	int result = 0;
	pthread_t thread1;
	pthread_t thread2;

	BUFFER buffer;
	buffer.buff = NULL;
	buffer.size_buff = DELTA;

	buffer.buff = (char*) malloc(buffer.size_buff);
	memset(buffer.buff, 0, buffer.size_buff);

	// mutex initialization
	pthread_mutex_init(&mutex, NULL);
	for (i = 0; i < 5; i++)
	{
		result = pthread_create(&thread1, NULL, (void*) threadFunc1, &buffer);
		if (result != 0)
		{
			printf("Error create thread1\n");
		}
		result = pthread_create(&thread2, NULL, (void*) threadFunc2, &buffer);
		if (result != 0)
		{
			printf("Error create thread2\n");
		}
		pthread_join(thread2, NULL);
		pthread_join(thread1, NULL);
	}
	// destroy mutex
	pthread_mutex_destroy(&mutex);
	printf("%s\n", buffer.buff);
	return EXIT_SUCCESS;
}
