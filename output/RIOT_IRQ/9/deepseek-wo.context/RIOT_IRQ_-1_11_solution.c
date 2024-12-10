
#include <stdio.h>
#include <stdint.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_int counter = ATOMIC_VAR_INIT(0);

void irq_handler(void *arg)
{
    atomic_fetch_add(&counter, 1);
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Set the interrupt handler
    irq_set_handler(0, irq_handler, NULL);

    // Trigger the interrupt
    irq_trigger(0);

    // Wait for a short period to ensure the interrupt handler has run
    thread_sleep(100000);

    // Check the counter value
    int final_count = atomic_load(&counter);
    printf("Counter value after interrupt: %d\n", final_count);

    return 0;
}
