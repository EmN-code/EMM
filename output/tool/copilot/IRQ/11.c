#include "irq.h"
#include "atomic_utils.h"
#include "xtimer.h"
#include "thread.h"
#include "periph/uart.h"

static volatile uint32_t atomic_counter = 0;

void irq_handler(void *arg)
{
    (void)arg;
    // Increment the atomic counter
    atomic_fetch_add_u32(&atomic_counter, 1);
}

void disable_and_restore_interrupts(void)
{
    // Disable interrupts
    unsigned int state = irq_disable();

    // Critical section: modify the atomic counter
    atomic_fetch_add_u32(&atomic_counter, 1);

    // Restore interrupts
    irq_restore(state);
}

int main(void)
{
    // Initialize UART for debugging
    uart_init(UART_DEV(0), 115200, NULL, NULL);

    // Print initial value of atomic counter
    printf("Initial atomic counter value: %u\n", atomic_load_u32(&atomic_counter));

    // Set up an interrupt handler
    irq_enable();
    irq_add(IRQ_TIMER0, irq_handler, NULL);

    // Disable and restore interrupts while modifying the atomic counter
    disable_and_restore_interrupts();

    // Print final value of atomic counter
    printf("Final atomic counter value: %u\n", atomic_load_u32(&atomic_counter));

    return 0;
}