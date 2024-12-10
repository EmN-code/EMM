
#include <stdio.h>
#include "irq.h"
#include "thread.h"
#include "xtimer.h"

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE)

static kernel_pid_t thread_pid;
static char thread_stack[THREAD_STACKSIZE];

static void *thread_handler(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

static void irq_handler(void)
{
    printf("Interrupt triggered\n");
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Create a thread to run concurrently with the interrupt handler
    thread_pid = thread_create(thread_stack, sizeof(thread_stack),
                               THREAD_PRIORITY_MAIN - 1,
                               THREAD_CREATE_STACKTEST,
                               thread_handler, NULL, "thread");

    // Disable interrupts
    unsigned state = irq_disable();

    // Set the interrupt handler
    irq_set(0, irq_handler);

    // Enable interrupts
    irq_restore(state);

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        printf("Main loop iteration %d\n", i);
        xtimer_sleep(1);
    }

    return 0;
}
