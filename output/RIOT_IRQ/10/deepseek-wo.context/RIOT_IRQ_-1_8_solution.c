
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_int counter = ATOMIC_VAR_INIT(0);

void irq_handler(void *arg)
{
    (void)arg;
    atomic_store(&counter, 1);
}

int main(void)
{
    puts("Starting interrupt handling test...");

    // Set the interrupt handler
    irq_set_handler(IRQ_NUM, irq_handler, NULL);

    // Trigger the interrupt
    irq_trigger(IRQ_NUM);

    // Load the atomic variable to check if the interrupt was handled
    int value = atomic_load(&counter);

    if (value == 1) {
        puts("Interrupt was successfully handled.");
    } else {
        puts("Interrupt handling failed.");
    }

    return 0;
}
