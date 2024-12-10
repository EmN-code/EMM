
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_int_t atomic_var = ATOMIC_INIT(0);

void irq_handler(void *arg)
{
    (void)arg;
    printf("Interrupt occurred!\n");
    atomic_store(&atomic_var, 1);
}

int main(void)
{
    puts("RIOT atomic interrupt handling example");

    // Set the interrupt handler
    irq_set_handler(0, irq_handler, NULL);

    // Enable the interrupt
    irq_set(0, 1);

    // Wait for the interrupt to occur
    while (atomic_load(&atomic_var) == 0) {
        thread_yield();
    }

    puts("Interrupt handled successfully!");

    return 0;
}
