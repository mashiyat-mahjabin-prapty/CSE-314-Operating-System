/*Consider the following classical synchronization problem called the barbershop problem. A barbershop 
consists of a room with N chairs. If a customer enters the barbershop and all chairs are
occupied, then the customer leaves the shop. If the barber is busy, but chairs are available, then the
customer sits in one of the free chairs and awaits his turn. The barber moves onto the next waiting
seated customer after he finishes one hair cut. If there are no customers to be served, the barber
goes to sleep. If the barber is asleep when a customer arrives, the customer wakes up the barber to
give him a hair cut. A waiting customer vacates his chair after his hair cut completes.*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include <pthread.h>

pthread_cond_t customer, barber;
pthread_mutex_t lock;
int N;
int seats;
int freeSeats;

void *Barber()
{
    while(1)
    {
        pthread_mutex_lock(&lock);
        while( freeSeats == seats  ){

            pthread_cond_wait(&customer, &lock);
            if(N == 0) {
                 pthread_mutex_unlock(&lock);
                 return 0;
            }
        }
        
        printf("Barber gives a haircut\n");
        
        freeSeats++;
        
        pthread_cond_signal(&barber);
        pthread_mutex_unlock(&lock);

        
    }

    return 0;
}

void *Customer(void *data)
{
    int thread_id = *((int *)data);

    pthread_mutex_lock(&lock);

    if(freeSeats > 0)
    {
        printf("Customer %d is waiting for a haircut\n", thread_id);

        freeSeats--;
        pthread_cond_signal(&customer);
        
        pthread_cond_wait(&barber, &lock);
        pthread_mutex_unlock(&lock);
    }
    else{
        printf("Customer %d left Saloon without a haircut\n", thread_id);
        pthread_mutex_unlock(&lock);
    }
    
    
    return 0;
}

int main(int argc, char *argv[])
{
    N = 10;
    seats = 4;

    if (argc < 3)
    {
        printf("Not enough arguments given\n");
        exit(1);
    }
    else
    {
        N = atoi(argv[1]);
        seats = atoi(argv[2]);
    }
    freeSeats = seats;
    
    pthread_cond_init(&customer, NULL);
    pthread_cond_init(&barber, NULL);
    pthread_mutex_init(&lock, NULL);

    int *customer_thread_id;
    pthread_t barber_thread, *customer_thread;

    int i;

    pthread_create(&barber_thread, NULL, Barber, NULL);

    //create worker consumer threads
    customer_thread_id = (int *)malloc(sizeof(int) * N);
    customer_thread = (pthread_t *)malloc(sizeof(pthread_t) * N);
    for (i = 0; i < N; i++)
    customer_thread_id[i] = i;

    for (i = 0; i < N; i++)
    pthread_create(&customer_thread[i], NULL, Customer, (void *)&customer_thread_id[i]);

    

    for (i = 0; i < N; i++)
    {
        pthread_join(customer_thread[i], NULL);
        printf("customer %d joined\n", i);
    }

    N = 0;
    pthread_cond_signal(&customer); 
       
    pthread_join(barber_thread, NULL);
    printf("barber joined\n");

    /*----Deallocating Buffers---------------------*/
    free(customer_thread_id);
    free(customer_thread);

    return 0;
}