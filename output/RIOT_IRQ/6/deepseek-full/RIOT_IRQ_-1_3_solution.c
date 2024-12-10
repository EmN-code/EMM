
#include <stdio.h>
#include <stdint.h>
#include "irq.h"
#include "thread.h"
#include "event/thread.h"
#include "mma8x5x_regs.h"
#include "MQTTClient.h"

#define STACKSIZE 512
#define PRIORITY 5

static char stack[STACKSIZE];
static volatile int atomic_var = 0;

void interrupt_handler(void *arg)
{
    (void)arg;
    unsigned state = irq_disable();
    atomic_var = 1;
    irq_restore(state);
    printf("Interrupt handled, atomic variable set to 1\n");
}

int main(void)
{
    kernel_pid_t pid = thread_create(stack, STACKSIZE, PRIORITY, THREAD_CREATE_STACKTEST, interrupt_handler, NULL, "interrupt_handler");
    if (pid <= 0) {
        printf("Failed to create thread\n");
        return 1;
    }

    while (1) {
        unsigned state = irq_disable();
        if (atomic_var == 1) {
            atomic_var = 0;
            irq_restore(state);
            printf("Atomic variable reset to 0\n");
        } else {
            irq_restore(state);
        }
    }

    return 0;
}
