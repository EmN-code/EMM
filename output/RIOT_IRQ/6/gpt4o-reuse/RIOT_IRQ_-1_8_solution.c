
#include "xtimer.h"
#include "irq.h"
#include "thread.h"
#include "msg.h"
#include "periph/timer.h"

#define TIMER_DEV         TIMER_DEV(0)
#define TIMER_CHAN        (0)
#define TIMER_IRQ         (TIMER_DEV_CHAN_ISR(TIMER_DEV, TIMER_CHAN))

static volatile int atomic_var = 0;

static void timer_isr(void *arg)
{
    (void)arg;
    atomic_var++;
    printf("Interrupt occurred, atomic_var incremented to: %d\n", atomic_var);
}

int main(void)
{
    xtimer_init();

    /* Initialize the timer */
    timer_init(TIMER_DEV, 1000000U, timer_isr, NULL);

    /* Disable interrupts */
    unsigned state = irq_disable();

    /* Modify the atomic variable */
    atomic_var = 1;

    /* Restore interrupts */
    irq_restore(state);

    /* Enable the timer interrupt */
    timer_irq_enable(TIMER_DEV, TIMER_CHAN);

    /* Wait for the interrupt to occur */
    xtimer_sleep(5);

    /* Disable the timer interrupt */
    timer_irq_disable(TIMER_DEV, TIMER_CHAN);

    printf("Final value of atomic_var: %d\n", atomic_var);

    return 0;
}
