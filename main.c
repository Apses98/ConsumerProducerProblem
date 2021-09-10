/* Header and Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include <stdint.h>

/* Constants */
#define ZERO 0
#define ONE 1

/* Global variables */
sem_t isEmpty, isFull;
pthread_mutex_t mutexBuffer;
int timeinterval, num = ZERO, randomNr;
int* buffer = NULL;

/* Declarations of Pthread Routins/Functions */
void* producer(void* args);
void* consumer(void* args);


int main() {
	srand(time(NULL));

    
    int bufferSize, N;

    /* User inputs
    *
    *   Buffer Size
    *   The number of consumers
    *   The time interval
    *
     */

    
    printf("Ange bufferSize: ");
    scanf("%d", &bufferSize);  

    buffer = calloc(bufferSize, sizeof(int));
        
    printf("Ange antal konsumenter: ");
    scanf("%d", &N);
        
    /* While loop to prevent setting the time to ZERO */
    while(!timeinterval ){
      printf("Ange timeinterval: ");
      scanf("%d", &timeinterval);  
    }


    pthread_t thread[N];
    pthread_mutex_init(&mutexBuffer, NULL);
    sem_init(&isEmpty, ZERO, bufferSize);
    sem_init(&isFull, ZERO, ZERO);
        
    for (int i = ZERO; i < N + ONE; i++) {

        if (i > ZERO) {
         
            if (pthread_create(&thread[i], NULL, &consumer, (void *) (intptr_t) i ) != ZERO) {
            }
        } else {
            if (pthread_create(&thread[i], NULL, &producer, NULL) != ZERO) {
            }
        }
    }
    for (int i = ZERO; i < N; i++) {
        if (pthread_join(thread[i], NULL) != ZERO) {
        }
    }
    /* Destroy the semaphores, mutex and empty the memory */
    sem_destroy(&isEmpty);
    sem_destroy(&isFull);
    pthread_mutex_destroy(&mutexBuffer);
    free(buffer);
    buffer = NULL;


    return ZERO;
}




void* producer(void* args) {
	randomNr= rand() % 100;
	int x =  randomNr;
    
    while (ONE) {
        
        sleep(timeinterval);

        /* Check if buffer is empty, if so, wait */
        sem_wait(&isEmpty);
        /* Lock it until everything is done */
        pthread_mutex_lock(&mutexBuffer);
        /* Make the x equal to the first "Element" in the buffer array */
        buffer[num] = x;
        printf("Producenten producerade datan: %d\n", x);
        /* Increase the index of the buffer and increase X (The Data) */
        num++;
	x++;
        
        /* unlock it when everything is done */
        pthread_mutex_unlock(&mutexBuffer);
        sem_post(&isFull);
    }
}

/* Pthread rutines/functions */
void* consumer(void* args) {
  int consumerNr = (intptr_t) args;
    while (ONE) {
        int y;

        /* If the buffer is full , wait */
        sem_wait(&isFull);
         /* Lock it until everything is done */
        pthread_mutex_lock(&mutexBuffer);
        /* Declear the y as the element at the index num - 1 in the buffer array because we added 1 to "num" */
        y = buffer[num - ONE];
        /* Decrease the index "num" by one to tell the program that this a new place is empty */
        num--;
        /* Unlock it when everything is done */
        pthread_mutex_unlock(&mutexBuffer);
        /* Check if buffer is empty */
        sem_post(&isEmpty);

        /* Print the consumed data */
        printf("Konsumenten %d konsumerade datan: %d\n", consumerNr, y);
        sleep(timeinterval);
    }
}

/* End Of Program */
