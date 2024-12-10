#include "xtimer.h"
#include "ztimer.h"
#include "irq.h"

#define MAIN_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define MAIN_THREAD_STACKSIZE (THREAD_STACKSIZE_MAIN)

static volatile uint32_t count = 0;

// Function that will be executed on interrupt
void irq_handler(void) {
    count++;
}

int main(void) {
    // Register the IRQ handler
    irq_handler_init(irq_handler);

    // Disabling interrupts
    irq_disable();
    printf("Interrupts disabled.\n");

    // Simulate work during which interrupts should be blocked
    for (volatile int i = 0; i < 1000000; i++);

    // Verifying that interrupt was not handled during the critical section
    printf("Count should be 0: %u\n", count);

    // Enabling interrupts
    irq_enable();
    printf("Interrupts enabled.\n");

    // Allow some time for interrupt to occur
    ztimer_sleep(ZTIMER_MSEC, 1000);

    // Verifying that interrupt is handled after reenabling
    printf("Count should be increased: %u\n", count);

    return 0;
}