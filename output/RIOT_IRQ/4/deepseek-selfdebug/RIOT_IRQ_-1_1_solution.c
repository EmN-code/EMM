
#include "irq.h"
#include "thread.h"
#include "xtimer.h"
#include "atomic_utils.h"

#define THREAD_STACKSIZE (512)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

volatile atomic_t interrupt_flag = ATOMIC_INIT(0);

void interrupt_handler(void)
{
    atomic_store(&interrupt_flag, 1);
}

void test_thread(void)
{
    while (1) {
        unsigned state = irq_disable();
        if (atomic_load(&interrupt_flag) == 1) {
            printf("Interrupt was handled while interrupts were disabled!\n");
        } else {
            printf("Interrupts disabled successfully.\n");
        }
        irq_restore(state);

        xtimer_sleep(1);
    }
}

int main(void)
{
    thread_create(
        NULL,
        THREAD_STACKSIZE,
        THREAD_PRIORITY,
        THREAD_CREATE_WOUT_YIELD | THREAD_CREATE_STACKTEST,
        test_thread,
        NULL,
        "test_thread"
    );

    // Simulate an interrupt by calling the handler directly
    while (1) {
        xtimer_sleep(2);
        interrupt_handler();
    }

    return 0;
}
