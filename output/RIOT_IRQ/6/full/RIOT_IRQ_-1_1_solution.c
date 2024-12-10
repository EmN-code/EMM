#include "xtimer.h"
#include "irq.h"
#include "board.h"
#include "thread.h"
#include <stdio.h>

static volatile unsigned irq_status;
static volatile int counter;

void safe_increment(void) {
    irq_status = irq_disable();  // Disable interrupts
    printf("Interrupts disabled.\n");

    // Critical section
    counter++;
    printf("Counter incremented to %d\n", counter);

    irq_enable(irq_status);  // Restore interrupts
    printf("Interrupts restored.\n");
}

int main(void) {
    printf("RIOT Interrupt Handling Example on ESP32\n");
    while (1) {
        safe_increment();
        xtimer_sleep(1);
    }
    return 0;
}