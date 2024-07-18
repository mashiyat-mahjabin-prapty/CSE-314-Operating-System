/*Consider the following classical synchronization problem called the barbershop problem. A barbershop consists of a room with N chairs. If a customer enters the barbershop and all chairs are
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
#include "zemaphore.h"

zem_t customer, barber,  lock;
int N = 10;
int waiting = 0;
int seats = 4;
void *Barber()
{
    while(1)
    {
        zem_down(&barber);
        zem_up(&customer);
        
        if(N == 0)
        {
            return 0;
        }
        printf("Barber gives a haircut\n");
      //  zem_up(&barber);
        zem_down(&lock);
        waiting--;
        zem_up(&lock);
    }

    return 0;
}

void *Customer(void *data)
{
    int thread_id = *((int *)data);

    zem_down(&lock);

    if(waiting > seats)
    {
        zem_up(&lock);
        printf("Customer %d left Saloon without a haircut\n", thread_id);
        return 0;
    }
    waiting++;
    zem_up(&lock);

    printf("Customer %d waits for a haircut\n", thread_id);
    zem_up(&barber);
    zem_down(&customer);

    
    
    return 0;
}

int main(int argc, int argv[])
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
    zem_init(&lock, 1);
    zem_init(&barber, 0);
    zem_init(&customer, 0);
    int *customer_thread_id, i;
    pthread_t barber_thread, *customer_thread;
    
    // zem_up(&customer);
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
    zem_up(&barber);
    pthread_join(barber_thread, NULL);
    printf("barber joined\n");
    /*----Deallocating Buffers---------------------*/
    free(customer_thread_id);
    free(customer_thread);

    return 0;
}