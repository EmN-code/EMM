
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_int_t atomic_var = ATOMIC_INIT(0);

void irq_handler(void *arg)
{
    (void)arg;
    atomic_store(&atomic_var, 1);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Set the IRQ handler
    irq_set_handler(0, irq_handler, NULL);

    // Enable the IRQ
    irq_enable(0);

    // Trigger the IRQ
    irq_set(0);

    // Wait for the IRQ to be handled
    thread_sleep(100000);

    // Check the atomic variable
    int value = atomic_load(&atomic_var);
    if (value == 1) {
        printf("Interrupt was handled correctly.\n");
    } else {
        printf("Interrupt handling failed.\n");
    }

    return 0;
}
