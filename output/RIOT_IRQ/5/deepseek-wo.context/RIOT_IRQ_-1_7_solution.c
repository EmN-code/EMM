
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

#define IRQ_NUM 0  // Define the IRQ number to use

atomic_int_t atomic_var;  // Use atomic_int_t instead of atomic_bit_u8_t

void irq_handler(void *arg)
{
    (void)arg;
    printf("Interrupt occurred!\n");
    atomic_store(&atomic_var, 1);  // Use atomic_store instead of atomic_store_u8
}

int main(void)
{
    atomic_init(&atomic_var, 0);  // Use atomic_init instead of atomic_bit_u8

    printf("Disabling interrupts...\n");
    irq_disable();

    printf("Setting interrupt handler...\n");
    irq_set_handler(IRQ_NUM, irq_handler, NULL);  // Use irq_set_handler instead of irq_handler

    printf("Enabling interrupts...\n");
    irq_enable();

    printf("Waiting for interrupt...\n");
    while (atomic_load(&atomic_var) == 0) {  // Use atomic_load instead of atomic_load_u8
        thread_yield();
    }

    printf("Interrupt handled!\n");
    return 0;
}
