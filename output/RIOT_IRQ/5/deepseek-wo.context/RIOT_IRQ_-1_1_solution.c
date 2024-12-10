
#include "irq.h"
#include "thread.h"
#include "xtimer.h"
#include "atomic_utils.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

volatile atomic_t interrupt_flag = ATOMIC_INIT(0);

void interrupt_handler(void)
{
    atomic_set(&interrupt_flag, 1);
}

void test_interrupt_handling(void)
{
    unsigned state = irq_disable();
    if (atomic_get(&interrupt_flag) == 1) {
        printf("Interrupt was not disabled correctly.\n");
    } else {
        printf("Interrupt was disabled correctly.\n");
    }
    irq_restore(state);

    xtimer_sleep(1);  // Allow time for an interrupt to potentially occur

    if (atomic_get(&interrupt_flag) == 1) {
        printf("Interrupt was handled correctly.\n");
    } else {
        printf("Interrupt was not handled.\n");
    }
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    // Register the interrupt handler
    irq_set_handler(IRQ_NUM, interrupt_handler);

    // Test the interrupt handling mechanism
    test_interrupt_handling();

    return 0;
}
