
#include "xtimer.h"
#include "periph/timer.h"
#include "irq.h"
#include "thread.h"
#include "msg.h"

#define TIMER_DEV         (0)
#define TIMER_CHANNEL     (0)
#define TIMER_INTERVAL    (1000000U)  /* 1 second */

static volatile int interrupt_occurred = 0;

static void timer_callback(void *arg)
{
    (void)arg;
    interrupt_occurred = 1;
}

int main(void)
{
    timer_t timer = TIMER_DEV;
    int result = 0;

    /* Initialize the timer */
    result = timer_init(timer, TIMER_INTERVAL, timer_callback, NULL);
    if (result != 0) {
        printf("Timer initialization failed with error code: %d\n", result);
        return 1;
    }

    /* Disable interrupts */
    unsigned state = irq_disable();

    /* Enable the timer interrupt */
    timer_irq_enable(timer, TIMER_CHANNEL);

    /* Restore interrupts */
    irq_restore(state);

    /* Wait for the interrupt to occur */
    while (!interrupt_occurred) {
        thread_yield();
    }

    /* Disable the timer interrupt */
    timer_irq_disable(timer, TIMER_CHANNEL);

    printf("Interrupt occurred and handled successfully.\n");

    return 0;
}
