
#include "xtimer.h"
#include "irq.h"
#include "thread.h"
#include "atomic.h"

#define TIMER_INTERVAL (1000000U)  // 1 second in microseconds

static atomic_int_t atomic_var = ATOMIC_INIT(0);

static void timer_callback(void *arg)
{
    (void)arg;
    unsigned state = irq_disable();
    atomic_store(&atomic_var, 1);
    irq_restore(state);
}

int main(void)
{
    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    xtimer_set(&timer, TIMER_INTERVAL);

    while (1) {
        unsigned state = irq_disable();
        int value = atomic_load(&atomic_var);
        irq_restore(state);

        if (value == 1) {
            printf("Interrupt handled correctly, atomic variable set to 1\n");
            atomic_store(&atomic_var, 0);
        }

        thread_yield();
    }

    return 0;
}
