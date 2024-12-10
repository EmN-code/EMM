
#include <stdio.h>
#include <stdint.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>

#define IRQ_NUM 11

static volatile uint32_t atomic_var = 0;

static void irq_handler(void *arg)
{
    (void)arg;
    atomic_var++;
    printf("Interrupt handled, atomic_var incremented to: %u\n", atomic_var);
}

int main(void)
{
    printf("Starting interrupt handling test on ESP32\n");

    // Register the interrupt handler
    irq_disable(IRQ_NUM);
    irq_set_handler(IRQ_NUM, irq_handler, NULL);
    irq_enable(IRQ_NUM);

    // Disable interrupts and verify atomic variable
    irq_disable(IRQ_NUM);
    atomic_var = 0;
    xtimer_sleep(1); // Simulate some work
    if (atomic_var == 0) {
        printf("Interrupts disabled correctly, atomic_var is: %u\n", atomic_var);
    } else {
        printf("Interrupts were not disabled correctly, atomic_var is: %u\n", atomic_var);
    }
    irq_enable(IRQ_NUM);

    // Restore interrupts and verify atomic variable
    atomic_var = 0;
    xtimer_sleep(1); // Simulate some work
    if (atomic_var > 0) {
        printf("Interrupts restored correctly, atomic_var is: %u\n", atomic_var);
    } else {
        printf("Interrupts were not restored correctly, atomic_var is: %u\n", atomic_var);
    }

    printf("Interrupt handling test completed\n");
    return 0;
}
