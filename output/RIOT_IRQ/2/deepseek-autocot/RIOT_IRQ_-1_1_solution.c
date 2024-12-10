
#include "irq.h"
#include "thread.h"
#include "xtimer.h"
#include "atomic_utils.h"

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static volatile unsigned int atomic_var = 0;

void interrupt_handler(void)
{
    unsigned state = irq_disable();
    atomic_var++;
    irq_restore(state);
}

void worker_thread(void)
{
    while (1) {
        unsigned state = irq_disable();
        if (atomic_var == 0) {
            printf("Interrupts were disabled correctly.\n");
        } else {
            printf("Interrupts were not disabled correctly.\n");
        }
        irq_restore(state);
        xtimer_sleep(1);
    }
}

int main(void)
{
    thread_create(
        NULL,
        THREAD_STACKSIZE,
        THREAD_PRIORITY,
        THREAD_CREATE_WOUT_YIELD | THREAD_CREATE_STACKTEST,
        worker_thread,
        NULL,
        "worker_thread"
    );

    while (1) {
        interrupt_handler();
        xtimer_usleep(100);
    }

    return 0;
}
