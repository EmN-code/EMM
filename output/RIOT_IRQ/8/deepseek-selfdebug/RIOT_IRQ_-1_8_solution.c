
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>
#include <atomic_utils.h>

atomic_t flag = ATOMIC_INIT(0);

void interrupt_handler(void)
{
    atomic_store(&flag, 1);
}

void test_thread(void)
{
    while (1) {
        unsigned state = irq_disable();
        if (atomic_load(&flag) == 1) {
            printf("Interrupt was handled correctly.\n");
            atomic_store(&flag, 0);
        }
        irq_restore(state);
        xtimer_sleep(1);
    }
}

int main(void)
{
    puts("Starting interrupt handling test...");

    // Register the interrupt handler
    irq_set_handler(interrupt_handler);

    // Create a thread to test the interrupt handling
    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1, 0, test_thread, NULL, "test_thread");

    // Simulate an interrupt by calling the handler directly
    interrupt_handler();

    return 0;
}
