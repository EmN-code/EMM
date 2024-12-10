
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <timex.h>
#include <xtimer.h>
#include <ztimer.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <mbox.h>
#include <msg.h>
#include <thread.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <event/thread.h>
#include <stddef.h>
#include <time_units.h>

#define STACK_SIZE 1024
#define PRIORITY 5

static char stack[STACK_SIZE];
static volatile int atomic_var = 0;

void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_var = 1;
    printf("Interrupt handled, atomic_var set to 1\n");
}

void thread_func(void *arg)
{
    (void)arg;
    while (1) {
        if (atomic_var == 1) {
            printf("Atomic variable is set, disabling interrupts...\n");
            unsigned state = irq_disable();
            atomic_var = 0;
            printf("Atomic variable reset, restoring interrupts...\n");
            irq_restore(state);
        }
        xtimer_sleep(1);
    }
}

int main(void)
{
    kernel_pid_t pid = thread_create(stack, sizeof(stack), PRIORITY, 0, thread_func, NULL, "interrupt_thread");
    if (pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread\n");
        return 1;
    }

    irq_set_handler(IRQ_NUM, interrupt_handler, NULL);
    irq_enable(IRQ_NUM);

    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
