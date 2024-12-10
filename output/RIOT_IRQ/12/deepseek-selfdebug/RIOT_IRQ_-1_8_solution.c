
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

#define IRQ_NUM 0  // Define the IRQ number to use

atomic_int_t atomic_var;  // Use atomic_int_t instead of atomic_bit_u8_t

void irq_handler(void *arg)
{
    atomic_store(&atomic_var, 1);  // Use atomic_store instead of atomic_store_u8
    printf("Interrupt handled, atomic variable set to 1\n");
}

int main(void)
{
    atomic_init(&atomic_var, 0);  // Use atomic_init instead of ATOMIC_INIT

    printf("Disabling interrupts...\n");
    irq_disable();

    printf("Setting IRQ handler...\n");
    irq_set_handler(IRQ_NUM, irq_handler, NULL);  // Use irq_set_handler instead of irq_handler

    printf("Enabling IRQ...\n");
    irq_set(IRQ_NUM, 1);  // Use irq_set instead of irq_set_handler

    printf("Restoring interrupts...\n");
    irq_enable();

    printf("Waiting for interrupt...\n");
    while (atomic_load(&atomic_var) == 0) {  // Use atomic_load instead of atomic_load_u8
        thread_yield();
    }

    printf("Interrupt detected, atomic variable is %d\n", atomic_load(&atomic_var));

    return 0;
}
