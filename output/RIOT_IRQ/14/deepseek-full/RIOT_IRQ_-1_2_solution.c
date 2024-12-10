
#include <stdio.h>
#include "event/thread.h"
#include "thread.h"
#include "irq.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static char stack[STACKSIZE];
static kernel_pid_t pid;
static volatile int atomic_var = 0;

static void *thread_handler(void *arg)
{
    (void)arg;

    while (1) {
        printf("Thread running, atomic_var: %d\n", atomic_var);
        xtimer_sleep(1);
    }

    return NULL;
}

static void isr_handler(void)
{
    unsigned state = irq_disable();
    atomic_var = 1;
    irq_restore(state);
}

int main(void)
{
    pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                        THREAD_CREATE_STACKTEST, thread_handler, NULL, "thread");

    printf("Main thread running, atomic_var: %d\n", atomic_var);

    // Simulate an interrupt
    isr_handler();

    // Verify the atomic variable
    if (atomic_var == 1) {
        printf("Interrupt handling successful, atomic_var: %d\n", atomic_var);
    } else {
        printf("Interrupt handling failed, atomic_var: %d\n", atomic_var);
    }

    return 0;
}
