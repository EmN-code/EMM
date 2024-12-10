
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

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
    irq_set_handler(IRQ_NUM, irq_handler, NULL);

    // Trigger the interrupt
    irq_trigger(IRQ_NUM);

    // Wait for the interrupt to be handled
    thread_sleep(1000000);

    // Check the atomic variable
    if (atomic_load(&counter) == 1) {
        printf("Interrupt was handled correctly\n");
    } else {
        printf("Interrupt handling failed\n");
    }

    return 0;
}
