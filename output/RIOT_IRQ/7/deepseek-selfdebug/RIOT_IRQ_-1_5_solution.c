
#include <stdio.h>
#include <stdint.h>
#include <irq.h>
#include <thread.h>
#include <atomic_types.h>

atomic_uint_fast8_t atomic_var = ATOMIC_VAR_INIT(0);

void isr_handler(void *arg)
{
    (void)arg;
    atomic_store_u8(&atomic_var, 1);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Initialize the atomic variable
    atomic_store_u8(&atomic_var, 0);

    // Disable interrupts
    unsigned state = irq_disable();

    // Register the ISR
    irq_set(0, isr_handler, NULL);

    // Trigger the ISR
    irq_restore(state);

    // Wait for the ISR to set the atomic variable
    while (atomic_load_u8(&atomic_var) == 0) {
        thread_yield();
    }

    // Check if the ISR set the atomic variable
    if (atomic_load_u8(&atomic_var) == 1) {
        printf("Interrupt handling test passed!\n");
    } else {
        printf("Interrupt handling test failed!\n");
    }

    return 0;
}
