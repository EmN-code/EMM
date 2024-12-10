#include <ztimer/ztimer.h>
#include <irq.h>
#include <stdio.h>

volatile uint32_t counter = 0;
volatile uint32_t irq_disabled_counter = 0;

void test_irq_handling(void) {
    irq_disable();
    atomic_set(&irq_disabled_counter, irq_disabled_counter + 1);
    irq_restore();
    atomic_set(&counter, counter + 1);
}

int main(void) {
    printf("Starting irq handling test on ESP32\n");
    test_irq_handling();
    printf("Counter value: %lu, IRQ Disabled Counter value: %lu\n", (unsigned long)counter, (unsigned long)irq_disabled_counter);
    return 0;
}