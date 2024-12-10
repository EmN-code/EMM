
#include <stdio.h>
#include <thread.h>
#include <irq.h>
#include <atomic_utils.h>

atomic_int counter = ATOMIC_VAR_INIT(0);

void interrupt_handler(void)
{
    unsigned state = irq_disable();
    atomic_fetch_add(&counter, 1);
    irq_restore(state);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Register the interrupt handler
    irq_set_handler(interrupt_handler);

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        thread_sleep(1);
        printf("Counter: %d\n", atomic_load(&counter));
    }

    printf("Interrupt handling test completed.\n");
    return 0;
}
