#include <xtimer.h>
#include <periph/irq.h>
#include <stdbool.h>

// Atomic variable to check interrupt status
static volatile bool irq_disabled = false;

// Dummy interrupt service routine (ISR)
void irq_handler(void) {
    irq_disabled = true;  // Set the flag to indicate ISR execution
}

void test_irq_handling(void) {
    // Disable IRQ
    irq_disable();

    // Simulate an ISR call
    irq_handler();

    // Check if the irq_disabled variable is set
    if (irq_disabled) {
        printf("IRQ disabled and ISR executed successfully.\n");
    } else {
        printf("IRQ is not disabled or ISR execution failed.\n");
    }

    // Re-enable IRQ
    irq_enable();
}

int main(void) {
    // Test the IRQ handling
    test_irq_handling();
    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}