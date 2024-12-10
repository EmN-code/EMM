
#include <stdio.h>
#include "event/thread.h"
#include "irq.h"
#include "thread.h"
#include "mma8x5x_regs.h"
#include "MQTTClient.h"

#define STACKSIZE 512

static char stack[STACKSIZE];
static volatile int atomic_var = 0;

void interrupt_handler(void *arg)
{
    (void)arg;
    unsigned state = irq_disable();
    atomic_var = 1;
    irq_restore(state);
}

void *thread_func(void *arg)
{
    (void)arg;
    while (1) {
        thread_yield();
        if (atomic_var == 1) {
            puts("Interrupt handled correctly.");
            atomic_var = 0;
        }
    }
    return NULL;
}

int main(void)
{
    kernel_pid_t pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                                     THREAD_CREATE_STACKTEST, thread_func, NULL, "handler");
    if (pid <= KERNEL_PID_UNDEF) {
        puts("Failed to create thread.");
        return 1;
    }

    irq_set_handler(IRQ_NUM, interrupt_handler, NULL);
    irq_enable(IRQ_NUM);

    puts("Thread and interrupt handler set up. Waiting for interrupt...");

    return 0;
}
