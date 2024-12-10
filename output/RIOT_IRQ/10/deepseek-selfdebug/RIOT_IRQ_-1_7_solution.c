
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

#define IRQ_NUM 0  // Define the IRQ number to use

atomic_int_t atomic_var;  // Use atomic_int_t instead of atomic_bit_u8_t

void irq_handler(void *arg)
{
    (void)arg;
    atomic_store(&atomic_var, 1);  // Use atomic_store instead of atomic_store_u8
}

int main(void)
{
    atomic_init(&atomic_var, 0);  // Use atomic_init instead of ATOMIC_INIT

    printf("Disabling interrupts...\n");
    unsigned state = irq_disable();

    printf("Setting IRQ handler...\n");
    irq_set_handler(IRQ_NUM, irq_handler, NULL);  // Use irq_set_handler instead of irq_handler

    printf("Restoring interrupts...\n");
    irq_restore(state);

    printf("Triggering IRQ...\n");
    irq_trigger(IRQ_NUM);

    printf("Waiting for IRQ to be handled...\n");
    thread_sleep(1000000);  // Sleep for 1 second

    int value = atomic_load(&atomic_var);  // Use atomic_load instead of atomic_load_u8
    if (value == 1) {
        printf("IRQ was handled correctly.\n");
    } else {
        printf("IRQ was not handled correctly.\n");
    }

    return 0;
}
