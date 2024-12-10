#include <xtimer.h>  // for delays
#include <stdio.h>   // for printf
#include <stdint.h>  // for uint32_t
#include <irq.h>     // for irq handling
#include <atomic.h>  // for atomic operations

atomic_t irq_count;

void my_irq_handler(void *arg) {
    (void)arg; // Unused
    atomic_fetch_add(&irq_count, 1);
    printf("Interrupt handled, irq_count: %ld\n", atomic_load(&irq_count));
}

void some_function_that_triggers_irq(void) {
    irq_disable();
    my_irq_handler(NULL);
    irq_enable();
}

int main(void) {
    atomic_init(&irq_count, 0);

    printf("Starting IRQ test on ESP32\n");

    for (int i = 0; i < 5; i++) {
        some_function_that_triggers_irq();
        xtimer_sleep(1); // delay between interrupts
    }

    printf("Final irq_count: %ld\n", atomic_load(&irq_count));

    return 0;
}