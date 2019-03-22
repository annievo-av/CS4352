#include <stdio.h> // Standard Input/Output header
#include <stdlib.h> // Standard Library header
#include <pthread.h> // Standard pthread header

#define MAX 2 // # of item  to produce

pthread_mutex_t the_mutex;
pthread_cond_t condc, condp;
int consumeTimes[MAX] = { 1, 4, 3 };
int buffer = 0;

void* producer(void *ptr) 
{
    int i;

    for (i = 1; i <= MAX; i++) 
    {
        // Protect buffer
        pthread_mutex_lock(&the_mutex);

		while (buffer != 0)
			pthread_cond_wait(&condp, &the_mutex);

		buffer = i;	
        printf("Producer: Produced item %d \n", i);
        buffer++;
        
        // Protect buffer and release it
        pthread_cond_signal(&condc);
        pthread_mutex_unlock(&the_mutex);
        sleep(3);
    }
    pthread_exit(0);
}

void* consumer(void *ptr) 
{
    int i;
    
    for (i = 1; i <= MAX; i++) 
    {
        // Protect buffer
        pthread_mutex_lock(&the_mutex);
        
        // If there is nothing in the buffer then wait
        while (buffer == 0)
            pthread_cond_wait(&condc, &the_mutex);
        
		// Take item out of buffer
		buffer--;
        sleep(consumeTimes[i]);
        printf("Consumer: Consumed item %d\n", i);

		// Release the buffer
    	buffer = 0;
		pthread_cond_signal(&condp);
        pthread_mutex_unlock(&the_mutex);
    }
    pthread_exit(0);
}

int main(int argc, char **argv) 
{
    pthread_t pro, con;

    // Initialize the mutex and condition variables
    pthread_mutex_init(&the_mutex, NULL);
    
    // Initialize consumer condition variable
    pthread_cond_init(&condc, NULL);
    
    // Initialize producer condition variable
    pthread_cond_init(&condp, NULL);

    // Create the threads
    pthread_create(&con, NULL, consumer, NULL);
    pthread_create(&pro, NULL, producer, NULL);

    // Wait for the threads to finish
    pthread_join(pro, NULL);
	pthread_join(con, NULL);

    // Cleanup -- would happen automatically at end of program
    pthread_cond_destroy(&condc);
    pthread_cond_destroy(&condp);
	pthread_mutex_destroy(&the_mutex);
}

