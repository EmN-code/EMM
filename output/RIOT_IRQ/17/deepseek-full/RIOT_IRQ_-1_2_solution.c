
#include "event/thread.h"
#include "irq.h"
#include "thread.h"

#define STACKSIZE (1024)

static char thread_stack[STACKSIZE];
static volatile int atomic_var = 0;

static void *worker_thread(void *arg)
{
    (void)arg;

    // Simulate some work
    for (int i = 0; i < 1000000; i++) {
        atomic_var++;
    }

    return NULL;
}

static void irq_handler(void)
{
    // Simulate an interrupt
    atomic_var += 100;
}

int main(void)
{
    // Create a worker thread
    thread_create(thread_stack, sizeof(thread_stack),
                  THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  worker_thread, NULL, "worker");

    // Disable interrupts
    unsigned state = irq_disable();

    // Modify the atomic variable
    atomic_var = 50;

    // Restore interrupts
    irq_restore(state);

    // Set the interrupt handler
    irq_set_handler(0, irq_handler);

    // Enable the interrupt
    irq_enable(0);

    // Wait for the worker thread to finish
    thread_sleep();

    // Verify the atomic variable
    if (atomic_var == 1000150) {
        printf("Interrupt handling and atomic variable modification verified correctly.\n");
    } else {
        printf("Error in interrupt handling or atomic variable modification.\n");
    }

    return 0;
}
