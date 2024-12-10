
#include <stdio.h>
#include <stdint.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>

#define IRQ_NUM 11

static volatile int atomic_var = 0;

static void irq_handler(void *arg)
{
    (void)arg;
    atomic_var = 1;
    printf("Interrupt handled, atomic_var set to 1\n");
}

int main(void)
{
    printf("Starting interrupt handling test on ESP32\n");

    // Register the interrupt handler
    irq_disable(IRQ_NUM);
    irq_set_handler(IRQ_NUM, irq_handler, NULL);
    irq_enable(IRQ_NUM);

    // Test disabling and restoring interrupts
    printf("Disabling interrupts...\n");
    irq_disable(IRQ_NUM);
    xtimer_sleep(2); // Simulate some work
    printf("Restoring interrupts...\n");
    irq_enable(IRQ_NUM);

    // Simulate an interrupt
    printf("Triggering interrupt...\n");
    // Assuming some hardware trigger to simulate the interrupt
    // For example, a timer or a specific GPIO pin toggle

    // Wait for the interrupt to be handled
    while (atomic_var == 0) {
        xtimer_usleep(100);
    }

    printf("Interrupt handling test completed\n");
    return 0;
}
