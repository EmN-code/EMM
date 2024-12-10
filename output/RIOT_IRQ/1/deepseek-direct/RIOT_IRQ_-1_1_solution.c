
#include "xtimer.h"
#include "irq.h"
#include "thread.h"
#include "periph/cpuid.h"
#include "periph/pm.h"
#include "periph/timer.h"

#define TIMER_DEV         TIMER_DEV(0)
#define TIMER_CHANNEL     0
#define TIMER_IRQ_PRIO    1
#define TIMER_INTERVAL    (1000U)  /* 1 ms */

static volatile int atomic_var = 0;
static volatile int interrupt_occurred = 0;

static void timer_isr(void *arg)
{
    (void)arg;
    timer_clear(TIMER_DEV, TIMER_CHANNEL);
    interrupt_occurred = 1;

    // Disable interrupts
    unsigned state = irq_disable();

    // Modify the atomic variable
    atomic_var++;

    // Restore interrupts
    irq_restore(state);
}

int main(void)
{
    // Initialize the timer
    timer_init(TIMER_DEV, TIMER_INTERVAL, timer_isr, NULL);

    // Start the timer
    timer_start(TIMER_DEV);

    // Wait for the interrupt to occur
    while (!interrupt_occurred) {
        xtimer_sleep(1);
    }

    // Check the atomic variable
    if (atomic_var == 1) {
        printf("Interrupt handling and atomic variable modification successful!\n");
    } else {
        printf("Interrupt handling failed!\n");
    }

    // Stop the timer
    timer_stop(TIMER_DEV);

    return 0;
}
