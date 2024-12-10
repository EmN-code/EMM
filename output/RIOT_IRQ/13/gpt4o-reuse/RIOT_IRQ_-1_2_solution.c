
#include <stdio.h>
#include <stdint.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>

#define TEST_IRQ (11)  // Assuming IRQ 11 is available on ESP32

static volatile uint32_t atomic_var = 0;

void irq_handler(void *arg)
{
    (void)arg;
    printf("Interrupt triggered!\n");
    atomic_var++;  // Increment atomic variable
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Register the interrupt handler
    irq_disable(TEST_IRQ);
    irq_set(TEST_IRQ, irq_handler, NULL);
    irq_enable(TEST_IRQ);

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        xtimer_sleep(1);  // Sleep for 1 second
        printf("Atomic variable value: %u\n", atomic_var);
    }

    // Disable the interrupt
    irq_disable(TEST_IRQ);
    printf("Interrupt disabled. Atomic variable value: %u\n", atomic_var);

    return 0;
}
