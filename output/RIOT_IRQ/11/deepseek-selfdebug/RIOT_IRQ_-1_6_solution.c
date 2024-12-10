
#include <stdio.h>
#include <stdint.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

#define IRQ_NUM 1  // Define the IRQ number to use

atomic_int_t atomic_var;  // Use atomic_int_t instead of atomic_bit_u8_t

void irq_handler(void *arg)
{
    atomic_store(&atomic_var, 1);  // Use atomic_store instead of atomic_store_u8
    printf("Interrupt handled!\n");
}

int main(void)
{
    atomic_init(&atomic_var, 0);  // Use atomic_init instead of atomic_bit_u8

    // Set the interrupt handler
    irq_set_handler(IRQ_NUM, irq_handler, NULL);

    // Enable the interrupt
    irq_enable(IRQ_NUM);

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        printf("Working... %d\n", i);
        thread_sleep(1000000);  // Use thread_sleep with the correct number of arguments
    }

    // Check the atomic variable
    int value = atomic_load(&atomic_var);  // Use atomic_load instead of atomic_load_u8
    if (value == 1) {
        printf("Interrupt was successfully handled.\n");
    } else {
        printf("Interrupt was not handled.\n");
    }

    return 0;
}
