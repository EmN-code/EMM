#include "irq.h"
#include "atomic_utils.h"
#include "xtimer.h"
#include <stdio.h>

static volatile uint32_t interrupt_counter = 0;
static volatile uint32_t atomic_var = 0;

void isr_handler(void *arg)
{
    (void)arg;
    atomic_fetch_add_u32(&interrupt_counter, 1);
    printf("Interrupt occurred! Counter: %u\n", interrupt_counter);
}

void setup_interrupt(void)
{
    // Configure the interrupt source and handler
    int irq_num = 0; // Replace with the actual interrupt number
    irq_disable();
    irq_set(irq_num, isr_handler, NULL);
    irq_enable(irq_num);
    irq_restore();
}

int main(void)
{
    printf("Starting RIOT application on ESP32\n");

    // Initialize atomic variable
    atomic_store_u32(&atomic_var, 0);

    // Setup interrupt
    setup_interrupt();

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        uint32_t state = irq_disable();
        atomic_fetch_add_u32(&atomic_var, 1);
        irq_restore(state);
        xtimer_sleep(1);
    }

    printf("Final atomic variable value: %u\n", atomic_load_u32(&atomic_var));
    return 0;
}