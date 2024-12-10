
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

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        xtimer_sleep(1);
        printf("Main thread working, atomic_var: %u\n", atomic_var);
    }

    // Disable and re-enable the interrupt to verify atomicity
    irq_disable(IRQ_NUM);
    printf("Interrupt disabled, atomic_var: %u\n", atomic_var);
    xtimer_sleep(2);
    printf("After sleep with interrupt disabled, atomic_var: %u\n", atomic_var);
    irq_enable(IRQ_NUM);

    // Simulate more work
    for (int i = 0; i < 10; i++) {
        xtimer_sleep(1);
        printf("Main thread working, atomic_var: %u\n", atomic_var);
    }

    printf("Interrupt handling test completed\n");
    return 0;
}
