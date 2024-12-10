
#include "irq.h"
#include "thread.h"
#include "xtimer.h"
#include "atomic_utils.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

volatile atomic_t interrupt_flag = ATOMIC_INIT(0);

void irq_handler(void *arg)
{
    (void)arg;
    atomic_store(&interrupt_flag, 1);
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    // Set the interrupt handler
    irq_set_handler(0, irq_handler, NULL);

    // Enable the interrupt
    irq_enable(0);

    // Simulate an interrupt by setting the interrupt flag
    irq_set(0);

    // Wait for the interrupt to be handled
    while (atomic_load(&interrupt_flag) == 0) {
        xtimer_sleep(1);
    }

    // Check if the interrupt was handled correctly
    if (atomic_load(&interrupt_flag) == 1) {
        printf("Interrupt handled correctly.\n");
    } else {
        printf("Interrupt handling failed.\n");
    }

    return 0;
}
