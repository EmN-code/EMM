
#include <stdio.h>
#include "xtimer.h"
#include "thread.h"
#include "irq.h"

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char thread_stack[THREAD_STACKSIZE];
static kernel_pid_t thread_pid;
static volatile int atomic_var = 0;

void *thread_handler(void *arg)
{
    (void)arg;

    while (1) {
        if (atomic_var == 1) {
            printf("Interrupt handled, atomic_var is 1\n");
            atomic_var = 0;
        }
        xtimer_sleep(1);
    }

    return NULL;
}

void irq_handler(void)
{
    atomic_var = 1;
    printf("Interrupt triggered\n");
}

int main(void)
{
    thread_pid = thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY,
                               THREAD_CREATE_STACKTEST, thread_handler, NULL, "irq_handler");

    printf("Starting interrupt test\n");

    // Disable interrupts
    unsigned state = irq_disable();

    // Simulate an interrupt
    irq_handler();

    // Restore interrupts
    irq_restore(state);

    // Wait for the thread to handle the interrupt
    xtimer_sleep(2);

    printf("Interrupt test completed\n");

    return 0;
}
