
#include "thread.h"
#include "irq.h"
#include "xtimer.h"
#include "atomic_utils.h"

#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static volatile atomic_int_t atomic_var = ATOMIC_INIT(0);

static void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_store(&atomic_var, 1);
}

int main(void)
{
    kernel_pid_t pid;
    char stack[STACK_SIZE];

    /* Create a thread to handle the interrupt */
    pid = thread_create(stack, sizeof(stack), PRIORITY, THREAD_CREATE_WOUT_YIELD, interrupt_handler, NULL, "interrupt_handler");

    /* Set the interrupt handler */
    irq_set_handler(0, interrupt_handler, NULL);

    /* Enable the interrupt */
    irq_enable(0);

    /* Simulate some work */
    xtimer_sleep(1);

    /* Disable the interrupt */
    irq_disable(0);

    /* Check the atomic variable */
    if (atomic_load(&atomic_var) == 1) {
        puts("Interrupt was handled correctly.");
    } else {
        puts("Interrupt was not handled.");
    }

    return 0;
}
