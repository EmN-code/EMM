
#include "thread.h"
#include "irq.h"
#include "xtimer.h"
#include "atomic_utils.h"

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char thread_stack[THREAD_STACKSIZE];
static kernel_pid_t thread_pid;

static volatile atomic_t atomic_var = ATOMIC_INIT(0);

static void *thread_func(void *arg)
{
    (void)arg;

    while (1) {
        atomic_store(&atomic_var, 1);
        xtimer_sleep(1);
        atomic_store(&atomic_var, 0);
        xtimer_sleep(1);
    }

    return NULL;
}

static void irq_handler(void *arg)
{
    (void)arg;

    unsigned state = irq_disable();
    if (atomic_load(&atomic_var) == 1) {
        printf("Interrupt occurred while atomic variable is 1\n");
    } else {
        printf("Interrupt occurred while atomic variable is 0\n");
    }
    irq_restore(state);
}

int main(void)
{
    // Create the thread
    thread_pid = thread_create(thread_stack, sizeof(thread_stack),
                               THREAD_PRIORITY, THREAD_CREATE_WOUT_YIELD,
                               thread_func, NULL, "atomic_thread");

    // Set the interrupt handler
    irq_set_handler(0, irq_handler, NULL);

    // Enable the interrupt
    irq_set(0, 1);

    return 0;
}
