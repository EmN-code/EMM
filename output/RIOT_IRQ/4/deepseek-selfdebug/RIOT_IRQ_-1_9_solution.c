
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_bit_u8_t flag = ATOMIC_BIT_U8_INIT_ZERO;

void irq_handler(void *arg)
{
    (void)arg;
    atomic_store_u8(&flag, 1);
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Initialize the atomic variable
    atomic_init_u8(&flag, 0);

    // Set the interrupt handler
    irq_set_handler(IRQ_NUM, irq_handler, NULL);

    // Trigger the interrupt
    irq_trigger(IRQ_NUM);

    // Sleep for a short period to allow the interrupt to be handled
    thread_sleep();

    // Check the atomic variable to verify the interrupt was handled
    if (atomic_load_u8(&flag) == 1) {
        printf("Interrupt was successfully handled\n");
    } else {
        printf("Interrupt handling failed\n");
    }

    return 0;
}
