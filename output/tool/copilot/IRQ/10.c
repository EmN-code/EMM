#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdatomic.h>
#include "irq.h"
#include "thread.h"
#include "xtimer.h"

#define INTERRUPT_INTERVAL (1000000U) // 1 second

static atomic_bool interrupt_flag = false;

void isr_handler(void *arg)
{
    (void)arg;
    // Disable interrupts
    uint32_t state = irq_disable();
    
    // Set the atomic flag
    atomic_store(&interrupt_flag, true);
    
    // Restore interrupts
    irq_restore(state);
}

void *thread_handler(void *arg)
{
    (void)arg;
    while (1) {
        if (atomic_load(&interrupt_flag)) {
            printf("Interrupt handled!\n");
            atomic_store(&interrupt_flag, false);
        }
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void)
{
    // Create a thread to handle the interrupt flag
    char thread_stack[THREAD_STACKSIZE_MAIN];
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, thread_handler, NULL, "thread_handler");

    // Set up the interrupt handler
    irq_enable();
    irq_set(IRQ_TIMER0, isr_handler, NULL);

    // Simulate an interrupt every second
    while (1) {
        xtimer_usleep(INTERRUPT_INTERVAL);
        irq_invoke(IRQ_TIMER0);
    }

    return 0;
}