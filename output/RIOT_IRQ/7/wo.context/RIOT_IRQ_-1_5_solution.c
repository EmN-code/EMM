#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <irq.h>
#include <atomic.h>

atomic_t irq_state;

static void handler(void)
{
    // Logic to manage the interrupt goes here
    printf("Interrupt handled with counter: %d\n", irq_state);
}

int main(void)
{
    puts("RIOT ESP32 Interrupt Handling Example");

    atomic_set(&irq_state, 0);

    // Disabling interrupts
    irq_disable();
    xtimer_ticks64_t last_ticks = xtimer_ticks64(&xtimer_usec_timer);
    // Simulate some processing that requires interrupts to be off
    printf("Processing with interrupts disabled...\n");
    xtimer_usleep(1000000); // Sleep for 1 second

    // Enabling interrupts
    irq_enable();
    xtimer_ticks64_t current_ticks = xtimer_ticks64(&xtimer_usec_timer);

    // Check if the value changed during interrupt handling
    if (xtimer_usec_from_ticks64(current_ticks - last_ticks) > 0) {
        atomic_inc(&irq_state);
        handler();
    }
    else {
        printf("No interrupts were handled during this time.\n");
    }

    return 0;
}