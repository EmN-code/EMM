
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "event/thread.h"
#include "irq.h"
#include "thread.h"

#define STACKSIZE 512
#define PRIORITY THREAD_PRIORITY_MAIN - 1

static char stack[STACKSIZE];
static volatile int atomic_var = 0;

void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_var = 1;
}

void *thread_func(void *arg)
{
    (void)arg;
    while (1) {
        if (atomic_var == 1) {
            printf("Interrupt handled, atomic_var: %d\n", atomic_var);
            atomic_var = 0;
        }
    }
    return NULL;
}

int main(void)
{
    kernel_pid_t pid = thread_create(stack, sizeof(stack), PRIORITY,
                                    THREAD_CREATE_STACKTEST, thread_func, NULL, "thread");

    if (pid <= 0) {
        printf("Failed to create thread\n");
        return 1;
    }

    irq_set_handler(IRQ_NUM, interrupt_handler, NULL);
    irq_enable(IRQ_NUM);

    while (1) {
        printf("Main loop running\n");
        xtimer_sleep(1);
    }

    return 0;
}
