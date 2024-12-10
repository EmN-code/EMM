
#include <stdio.h>
#include <irq.h>
#include <mma8x5x_regs.h>

// Define a simple atomic variable type and operations
typedef volatile int atomic_t;

#define ATOMIC_INIT(val) (val)

static inline void atomic_store(atomic_t *var, int val) {
    *var = val;
}

static inline int atomic_load(atomic_t *var) {
    return *var;
}

atomic_t atomic_var = ATOMIC_INIT(0);

void interrupt_handler(void) {
    unsigned state = irq_disable();
    atomic_store(&atomic_var, 1);
    irq_restore(state);
}

int main(void) {
    printf("Starting interrupt handling test...\n");

    // Simulate enabling an interrupt (e.g., UART RX interrupt)
    uart_rxstart_irq_disable(UART_DEV(0)); // Disable UART RX interrupt

    // Simulate an interrupt occurring
    interrupt_handler();

    // Check the atomic variable to verify interrupt handling
    if (atomic_load(&atomic_var) == 1) {
        printf("Interrupt was handled correctly.\n");
    } else {
        printf("Interrupt handling failed.\n");
    }

    return 0;
}
