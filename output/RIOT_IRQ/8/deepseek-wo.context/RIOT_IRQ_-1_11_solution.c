
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>
#include <atomic_utils.h>

atomic_int counter = ATOMIC_VAR_INIT(0);

void irq_handler(void *arg)
{
    (void)arg;
    atomic_store(&counter, 1);
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Set the interrupt handler
    irq_set_handler(IRQ_NUM_EXTI0, irq_handler, NULL);

    // Enable the interrupt
    irq_enable(IRQ_NUM_EXTI0);

    // Wait for the interrupt to occur
    xtimer_sleep(1);

    // Check the atomic variable
    int value = atomic_load(&counter);
    if (value == 1) {
        printf("Interrupt was handled correctly\n");
    } else {
        printf("Interrupt was not handled\n");
    }

    return 0;
}
