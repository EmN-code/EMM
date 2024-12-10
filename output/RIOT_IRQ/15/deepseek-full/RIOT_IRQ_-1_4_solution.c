
#include <evtimer_msg.h>
#include <stdio.h>
#include <paho_mqtt.h>
#include <stdlib.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <stdatomic.h>
#include <xtimer.h>
#include <unistd.h>
#include <time_units.h>
#include <event/thread.h>
#include <stdbool.h>
#include <MQTTClient.h>
#include <irq.h>
#include <msg.h>
#include <thread.h>
#include <mbox.h>
#include <stddef.h>
#include <ztimer.h>

#define STACKSIZE (1024)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACKSIZE];
static atomic_int interrupt_flag = ATOMIC_VAR_INIT(0);

void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_store(&interrupt_flag, 1);
}

void *thread_func(void *arg)
{
    (void)arg;
    while (1) {
        if (atomic_load(&interrupt_flag) == 1) {
            printf("Interrupt detected, disabling interrupts...\n");
            unsigned state = irq_disable();
            printf("Interrupts disabled, entering critical section...\n");
            xtimer_sleep(1); // Simulate critical section
            printf("Exiting critical section, restoring interrupts...\n");
            irq_restore(state);
            atomic_store(&interrupt_flag, 0);
        }
        thread_yield();
    }
    return NULL;
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    kernel_pid_t pid = thread_create(stack, sizeof(stack), PRIORITY, THREAD_CREATE_STACKTEST, thread_func, NULL, "interrupt_thread");
    if (pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread\n");
        return 1;
    }

    // Simulate an interrupt
    xtimer_sleep(2);
    interrupt_handler(NULL);

    return 0;
}
