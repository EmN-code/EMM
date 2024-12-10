#include <stdio.h> 
#include <stdbool.h>
#include <irq_arch.h>

volatile int atomic_counter = 0;

void increment_counter(void) {
    unsigned state = irq_disable();
    atomic_counter++;
    irq_restore(state);
}

int main(void) {
    printf("Initial Counter Value: %d\n", atomic_counter);
    increment_counter();
    printf("Counter Value after First Increment: %d\n", atomic_counter);
    increment_counter();
    printf("Counter Value after Second Increment: %d\n", atomic_counter);
    return 0;
}