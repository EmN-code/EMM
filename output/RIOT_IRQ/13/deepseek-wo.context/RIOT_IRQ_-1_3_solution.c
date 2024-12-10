
#include "irq.h"
#include "thread.h"
#include "atomic_utils.h"
#include "xtimer.h"

#define TEST_VALUE (42)

static atomic_int_t atomic_var = ATOMIC_INIT(0);

void isr_handler(void *arg)
{
    (void)arg;
    atomic_store(&atomic_var, TEST_VALUE);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Set the interrupt handler
    irq_set_handler(IRQ_NUM, isr_handler, NULL);

    // Enable the interrupt
    irq_enable(IRQ_NUM);

    // Trigger the interrupt
    irq_trigger(IRQ_NUM);

    // Wait for the interrupt to be handled
    xtimer_sleep(1);

    // Check the value of the atomic variable
    int value = atomic_load(&atomic_var);
    if (value == TEST_VALUE) {
        printf("Interrupt handling test passed!\n");
    } else {
        printf("Interrupt handling test failed! Expected %d, got %d\n", TEST_VALUE, value);
    }

    return 0;
}
