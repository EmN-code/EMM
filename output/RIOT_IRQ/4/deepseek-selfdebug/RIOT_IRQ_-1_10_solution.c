
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

#define IRQ_NUM 0  // Define the IRQ number to use

volatile atomic_uint_fast8_t atomic_var = ATOMIC_VAR_INIT(0);

void irq_handler(void *arg)
{
    (void)arg;
    printf("Interrupt triggered!\n");
    atomic_store_u8(&atomic_var, 1);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Initialize the atomic variable
    atomic_init_u8(&atomic_var, 0);

    // Set the IRQ handler
    irq_set_handler(IRQ_NUM, irq_handler, NULL);

    // Trigger the IRQ
    irq_trigger(IRQ_NUM);

    // Wait for the interrupt to be handled
    while (atomic_load_u8(&atomic_var) == 0) {
        thread_yield();
    }

    printf("Interrupt handling test completed.\n");

    return 0;
}
