
#include <stdio.h>
#include "event/thread.h"
#include "irq.h"
#include "thread.h"
#include "mma8x5x_regs.h"
#include "MQTTClient.h"

volatile int atomic_var = 0;

void worker_thread(void)
{
    while (1) {
        // Simulate some work
        printf("Worker thread is working...\n");
        thread_sleep(1000000); // Sleep for 1 second
    }
}

void interrupt_handler(void *arg)
{
    (void)arg;
    printf("Interrupt triggered!\n");
    atomic_var = 1; // Modify the atomic variable
}

int main(void)
{
    // Create a worker thread
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, worker_thread, NULL, "worker");

    // Disable interrupts
    unsigned state = irq_disable();

    // Set the interrupt handler
    irq_set_handler(IRQ_NUM, interrupt_handler, NULL);

    // Enable interrupts
    irq_enable(IRQ_NUM);

    // Restore interrupts to the previous state
    irq_restore(state);

    // Verify the atomic variable
    if (atomic_var == 1) {
        printf("Atomic variable modified correctly.\n");
    } else {
        printf("Atomic variable not modified correctly.\n");
    }

    return 0;
}
