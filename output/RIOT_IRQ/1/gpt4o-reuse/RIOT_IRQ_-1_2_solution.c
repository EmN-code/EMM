
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
    irq_set(IRQ_NUM, irq_handler, NULL);
    irq_enable(IRQ_NUM);

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        printf("Main loop iteration: %d\n", i);
        xtimer_sleep(1);
    }

    // Disable the interrupt
    irq_disable(IRQ_NUM);
    printf("Interrupt disabled, atomic_var: %u\n", atomic_var);

    // Enable the interrupt again
    irq_enable(IRQ_NUM);
    printf("Interrupt re-enabled\n");

    // Simulate more work
    for (int i = 0; i < 10; i++) {
        printf("Main loop iteration: %d\n", i);
        xtimer_sleep(1);
    }

    printf("End of interrupt handling test\n");
    return 0;
}
