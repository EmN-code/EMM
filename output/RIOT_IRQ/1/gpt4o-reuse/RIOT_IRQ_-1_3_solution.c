
#include <stdio.h>
#include "irq.h"
#include "thread.h"
#include "xtimer.h"

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE)

static kernel_pid_t irq_thread_pid;
static volatile int atomic_var = 0;

static void *irq_thread(void *arg)
{
    (void)arg;

    while (1) {
        if (atomic_var == 1) {
            printf("Interrupt handled correctly.\n");
            atomic_var = 0;
        }
        xtimer_sleep(1);
    }

    return NULL;
}

static void irq_handler(void *arg)
{
    (void)arg;

    unsigned state = irq_disable();
    atomic_var = 1;
    irq_restore(state);
}

int main(void)
{
    puts("Interrupt handling test application");

    // Create a thread to handle the interrupt
    irq_thread_pid = thread_create(
        thread_stack, sizeof(thread_stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        irq_thread, NULL, "irq_thread");

    // Register the interrupt handler
    irq_set_handler(0, irq_handler, NULL);

    // Enable the interrupt
    irq_enable(0);

    // Simulate an interrupt by setting the atomic variable
    xtimer_sleep(2);
    irq_handler(NULL);

    return 0;
}
