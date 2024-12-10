
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_types.h>

atomic_int atomic_var = ATOMIC_INIT(0);

void irq_handler(void *arg)
{
    atomic_store(&atomic_var, 1);
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Set the IRQ handler
    irq_set_handler(0, irq_handler, NULL);

    // Trigger the IRQ
    irq_trigger(0);

    // Sleep for a while to allow the interrupt to be handled
    thread_sleep(1000000);

    // Check the atomic variable
    int value = atomic_load(&atomic_var);
    if (value == 1) {
        printf("Interrupt was handled correctly\n");
    } else {
        printf("Interrupt handling failed\n");
    }

    return 0;
}
