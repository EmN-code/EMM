
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_bit_u8_t flag;

void isr(void *arg)
{
    (void)arg;
    atomic_store_u8(&flag, 1);
}

int main(void)
{
    atomic_bit_u8_t local_flag;

    atomic_store_u8(&flag, 0);

    printf("Disabling interrupts...\n");
    unsigned state = irq_disable();

    printf("Setting flag in ISR...\n");
    isr(NULL);

    printf("Restoring interrupts...\n");
    irq_restore(state);

    local_flag = atomic_load_u8(&flag);
    if (local_flag == 1) {
        printf("Interrupt handling and atomic operations worked correctly.\n");
    } else {
        printf("Interrupt handling or atomic operations failed.\n");
    }

    return 0;
}
