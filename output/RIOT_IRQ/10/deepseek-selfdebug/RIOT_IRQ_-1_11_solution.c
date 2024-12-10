
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_bit_u8_t atomic_var;

void irq_handler(void *arg)
{
    atomic_store_u8(&atomic_var, 1);
}

int main(void)
{
    atomic_bit_u8_init(&atomic_var, 0);

    irq_set_handler(0, irq_handler, NULL);

    printf("Disabling interrupts...\n");
    irq_disable();

    printf("Triggering interrupt...\n");
    irq_trigger(0);

    printf("Restoring interrupts...\n");
    irq_restore(irq_disable());

    thread_sleep();

    if (atomic_load_u8(&atomic_var) == 1) {
        printf("Interrupt was handled correctly.\n");
    } else {
        printf("Interrupt was not handled.\n");
    }

    return 0;
}
