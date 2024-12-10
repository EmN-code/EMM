
#include <stdio.h>
#include <stdint.h>
#include <irq.h>
#include <xtimer.h>

#define IRQ_NUM 11

static volatile uint32_t atomic_var = 0;

void irq_handler(void *arg)
{
    (void)arg;
    atomic_var = 1;
    printf("Interrupt handled, atomic_var set to 1\n");
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Register the interrupt handler
    irq_set(IRQ_NUM, irq_handler, NULL);

    // Disable interrupts
    irq_disable();
    printf("Interrupts disabled\n");

    // Modify the atomic variable
    atomic_var = 0;
    printf("atomic_var set to 0\n");

    // Enable interrupts
    irq_enable();
    printf("Interrupts enabled\n");

    // Wait for the interrupt to be handled
    while (atomic_var == 0) {
        xtimer_usleep(1000);
    }

    printf("Test completed\n");
    return 0;
}
