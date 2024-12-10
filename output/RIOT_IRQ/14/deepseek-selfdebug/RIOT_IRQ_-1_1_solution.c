
#include "irq.h"
#include "thread.h"
#include "xtimer.h"
#include "atomic_utils.h"

#define TEST_VALUE 0x12345678

volatile uint32_t atomic_var = TEST_VALUE;

void interrupt_handler(void)
{
    // Simulate an interrupt by modifying the atomic variable
    atomic_var = TEST_VALUE ^ 0xFFFFFFFF;
}

int main(void)
{
    // Disable interrupts initially
    unsigned state = irq_disable();

    // Modify the atomic variable in an atomic context
    atomic_var = TEST_VALUE;

    // Re-enable interrupts
    irq_restore(state);

    // Simulate an interrupt by calling the handler
    interrupt_handler();

    // Check if the atomic variable was modified correctly
    if (atomic_var == (TEST_VALUE ^ 0xFFFFFFFF)) {
        printf("Interrupt handling and atomic variable modification successful!\n");
    } else {
        printf("Interrupt handling or atomic variable modification failed!\n");
    }

    // Infinite loop to keep the program running
    while (1) {
        // Sleep for a while to avoid busy-waiting
        xtimer_sleep(1);
    }

    return 0;
}
