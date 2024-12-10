
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_types.h>

atomic_int_t counter = ATOMIC_INIT(0);

void irq_handler(void *arg)
{
    (void)arg;
    atomic_store(&counter, 1);
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Set the interrupt handler
    irq_set_handler(0, irq_handler, NULL);

    // Enable the interrupt
    irq_enable(0);

    // Sleep for a while to allow the interrupt to trigger
    thread_sleep(1000000);

    // Check the atomic variable
    int value = atomic_load(&counter);
    printf("Atomic variable value: %d\n", value);

    return 0;
}
