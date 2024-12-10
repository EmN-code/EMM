#include "irq.h"
#include "atomic_utils.h"
#include "xtimer.h"
#include "thread.h"
#include "periph/uart.h"
static volatile uint32_t counter = 0;
static volatile bool irq_disabled = false;
void isr_handler(void *arg)
{
    (void)arg;
    atomic_fetch_add_u32(&counter, 1);
}
void init_interrupt(void)
{
    // Disable interrupts
    uint32_t state = irq_disable();
    irq_disabled = true;

    // Set the interrupt handler
    irq_set(IRQ_TIMER0, isr_handler, NULL);
    irq_enable(IRQ_TIMER0);

    // Restore interrupts
    irq_restore(state);
    irq_disabled = false;
}
void *thread_handler(void *arg)
{
    (void)arg;
    while (1) {
        if (irq_disabled) {
            printf("Interrupts are disabled\n");
        } else {
            printf("Interrupts are enabled\n");
        }
        xtimer_sleep(1);
    }
    return NULL;
}
int main(void)
{
    // Initialize the interrupt
    init_interrupt();

    // Create a thread to verify the interrupt handling
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, thread_handler, NULL, "thread");

    // Main loop
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}