
#include <stdio.h>
#include <irq.h>
#include <atomic_utils.h>

#define IRQ_NUM 0  // Define the IRQ number to use

atomic_int_t atomic_var;  // Use atomic_int_t instead of atomic_bit_u8_t

void irq_handler(void *arg) {
    atomic_store(&atomic_var, 1);  // Use atomic_store instead of atomic_store_u8
    printf("Interrupt occurred, atomic variable set to 1\n");
}

int main(void) {
    atomic_init(&atomic_var, 0);  // Use atomic_init instead of atomic_bit_u8
    printf("Initial atomic variable value: %d\n", atomic_load(&atomic_var));  // Use atomic_load instead of atomic_load_u8

    irq_set_handler(IRQ_NUM, irq_handler, NULL);  // Set the IRQ handler

    // Disable interrupts and check the atomic variable
    irq_disable();
    printf("Interrupts disabled, atomic variable value: %d\n", atomic_load(&atomic_var));

    // Restore interrupts and check the atomic variable
    irq_restore(irq_enable());
    printf("Interrupts restored, atomic variable value: %d\n", atomic_load(&atomic_var));

    return 0;
}
