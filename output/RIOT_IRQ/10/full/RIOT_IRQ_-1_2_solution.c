#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stddef.h>
#include "thread.h"

#define STACK_SIZE 1024
#define PRIORITY_MAIN 5
#define PRIORITY_ISR 3

static atomic_bool interrupt_flag = ATOMIC_VAR_INIT(false);

void *isr_handler(void *arg) {
    (void)arg;
    while (1) {
        if (atomic_load_explicit(&interrupt_flag, memory_order_relaxed)) {
            atomic_store_explicit(&interrupt_flag, false, memory_order_relaxed);
            printf("ISR: Interrupt handled\n");
        }
        usleep(100000); // Simulate periodic interrupt check
    }
    return NULL;
}

int main(void) {
    char stack[STACK_SIZE];
    thread_create(stack, sizeof(stack), PRIORITY_ISR, 0, isr_handler, NULL, "ISR");
    
    while (1) {
        printf("Main: Simulating work\n");
        usleep(500000); // Simulate doing work
        printf("Main: Triggering interrupt\n");
        atomic_store_explicit(&interrupt_flag, true, memory_order_relaxed);
    }
    return 0;
}