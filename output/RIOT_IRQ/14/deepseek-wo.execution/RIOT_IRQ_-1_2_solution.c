
#include "irq.h"
#include "thread.h"
#include "event/thread.h"

static volatile int atomic_var = 0;

void uart_rx_callback(void *arg)
{
    (void)arg;
    atomic_var = 1;
}

int main(void)
{
    unsigned state;

    // Initialize UART and set up the callback for interrupt handling
    uart_init(UART_DEV(0), 115200, uart_rx_callback, NULL);

    // Disable interrupts and save the current state
    state = irq_disable();

    // Perform some critical section code
    atomic_var = 0;

    // Restore the interrupt state
    irq_restore(state);

    // Wait for the interrupt to occur
    while (atomic_var == 0) {
        thread_yield();
    }

    // Verify that the interrupt was handled correctly
    if (atomic_var == 1) {
        puts("Interrupt was handled correctly.");
    } else {
        puts("Interrupt handling failed.");
    }

    return 0;
}
