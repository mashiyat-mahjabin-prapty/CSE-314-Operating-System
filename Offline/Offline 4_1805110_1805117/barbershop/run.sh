#!/bin/bash

echo "Running barbershop with pthreads"
gcc barbershop.c -o barbershop -lpthread
echo "10 customers and 4 free seats"
./barbershop 10 4
echo "100 customers and 4 free seats"
./barbershop 100 4
echo "1 customers and 4 free seats"
./barbershop 1 4
echo "100 customers and 40 free seats"
./barbershop 100 40

echo "Running barbershop with zemaphore"
gcc barbershop.c zemaphore.c -o barbershop_zem -lpthread
echo "10 customers and 4 free seats"
./barbershop_zem 10 4
echo "100 customers and 4 free seats"
./barbershop_zem 100 4
echo "1 customers and 4 free seats"
./barbershop_zem 1 4
echo "100 customers and 40 free seats"
./barbershop_zem 100 40