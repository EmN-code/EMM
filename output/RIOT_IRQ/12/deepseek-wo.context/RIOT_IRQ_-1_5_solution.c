
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

    // Trigger the interrupt
    irq_trigger(0);

    // Wait for the interrupt to be handled
    while (atomic_load(&interrupt_flag) == 0) {
        xtimer_usleep(100);
    }

    // Verify that the interrupt was handled
    if (atomic_load(&interrupt_flag) == 1) {
        puts("Interrupt was successfully handled.");
    } else {
        puts("Interrupt handling failed.");
    }

    return 0;
}
