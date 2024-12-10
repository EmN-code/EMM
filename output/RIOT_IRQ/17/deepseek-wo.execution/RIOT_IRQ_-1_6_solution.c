
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <kernel_defines.h>
#include <xtimer.h>
#include <msg.h>
#include <thread.h>

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

atomic_int counter = ATOMIC_VAR_INIT(0);

void *thread_func(void *arg)
{
    (void)arg;
    while (1) {
        atomic_fetch_add(&counter, 1);
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void)
{
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    char stack[THREAD_STACKSIZE];
    kernel_pid_t pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY,
                                     THREAD_CREATE_STACKTEST, thread_func, NULL, "counter");

    unsigned state = irq_disable();
    printf("Counter value before enabling interrupts: %d\n", atomic_load(&counter));
    irq_restore(state);

    xtimer_sleep(5);

    state = irq_disable();
    printf("Counter value after enabling interrupts: %d\n", atomic_load(&counter));
    irq_restore(state);

    return 0;
}
