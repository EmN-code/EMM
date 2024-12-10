
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

#define IRQ_NUM 0  // Define the IRQ number to use

atomic_int_t atomic_var;  // Use atomic_int_t instead of atomic_t

void irq_handler(void)
{
    atomic_store(&atomic_var, 1);  // Use atomic_store instead of atomic_store_u8
    printf("Interrupt handled!\n");
}

int main(void)
{
    atomic_store(&atomic_var, 0);  // Initialize atomic variable

    printf("Disabling interrupts...\n");
    unsigned state = irq_disable();  // Disable interrupts

    printf("Setting IRQ handler...\n");
    irq_set_handler(IRQ_NUM, irq_handler);  // Set the IRQ handler

    printf("Restoring interrupts...\n");
    irq_restore(state);  // Restore interrupts

    while (1) {
        if (atomic_load(&atomic_var) == 1) {  // Use atomic_load instead of atomic_load_u8
            printf("Atomic variable verified: Interrupts work correctly!\n");
            break;
        }
    }

    return 0;
}
