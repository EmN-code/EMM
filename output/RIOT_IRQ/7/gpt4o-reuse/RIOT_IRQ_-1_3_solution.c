
#include <stdio.h>
#include "irq.h"
#include "thread.h"
#include "xtimer.h"

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE)

static char thread_stack[THREAD_STACKSIZE];
static kernel_pid_t thread_pid;
static volatile int atomic_var = 0;

static void *thread_func(void *arg)
{
    (void)arg;

    while (1) {
        xtimer_sleep(1);
        printf("Thread running, atomic_var: %d\n", atomic_var);
    }

    return NULL;
}

static void irq_handler(void)
{
    atomic_var++;
    printf("Interrupt handled, atomic_var incremented: %d\n", atomic_var);
}

int main(void)
{
    // Initialize the thread
    thread_pid = thread_create(thread_stack, sizeof(thread_stack),
                               THREAD_PRIORITY_MAIN - 1,
                               THREAD_CREATE_STACKTEST,
                               thread_func, NULL, "interrupt_thread");

    // Disable interrupts
    unsigned state = irq_disable();

    // Register the interrupt handler
    irq_set_handler(0, irq_handler);

    // Enable interrupts
    irq_restore(state);

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        xtimer_sleep(2);
        printf("Main loop iteration: %d\n", i);
    }

    return 0;
}
