#include <stdio.h>
#include <stdint.h>
#include <irq.h>
#include <inttypes.h>
#include <atomic.h>

static atomic_t interrupt_counter = ATOMIC_INIT(0);

void interrupt_handler(void) {
    irq_disable();
    atomic_add(&interrupt_counter, 1);
    irq_enable();
}

int main(void) {
    printf("Starting interrupt handling test.\n");

    /* Simulating an interrupt for demonstration purposes */
    printf("Triggering interrupt...\n");
    interrupt_handler();

    printf("Number of interrupts handled: %" PRIu32 "\n", atomic_get(&interrupt_counter));

    return 0;
}