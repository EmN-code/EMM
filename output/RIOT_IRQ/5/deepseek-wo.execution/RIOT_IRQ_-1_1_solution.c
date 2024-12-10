
#include "irq.h"
#include "thread.h"
#include "xtimer.h"
#include "atomic_utils.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

volatile atomic_t interrupt_flag = ATOMIC_INIT(0);

void interrupt_handler(void)
{
    atomic_store(&interrupt_flag, 1);
}

void test_interrupt_handling(void)
{
    unsigned state = irq_disable();
    atomic_store(&interrupt_flag, 0);
    irq_restore(state);

    xtimer_sleep(1);  // Simulate some delay to allow interrupt to occur

    if (atomic_load(&interrupt_flag) == 1) {
        puts("Interrupt was successfully handled.");
    } else {
        puts("Interrupt was not handled.");
    }
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    // Register the interrupt handler
    irq_set_handler(interrupt_handler);

    // Test the interrupt handling mechanism
    test_interrupt_handling();

    return 0;
}
