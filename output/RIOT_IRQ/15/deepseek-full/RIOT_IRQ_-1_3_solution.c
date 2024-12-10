
#include <stdio.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdlib.h>
#include "event/thread.h"
#include "irq.h"
#include "thread.h"

#define STACKSIZE 512

static char stack[STACKSIZE];
static atomic_bool interrupt_flag = ATOMIC_VAR_INIT(false);

static void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_store(&interrupt_flag, true);
    printf("Interrupt handled!\n");
}

static void *thread_func(void *arg)
{
    (void)arg;
    while (1) {
        if (atomic_load(&interrupt_flag)) {
            printf("Interrupt flag set, disabling interrupts...\n");
            unsigned state = irq_disable();
            printf("Interrupts disabled, performing critical section...\n");
            // Simulate critical section
            vTaskDelay(pdMS_TO_TICKS(1000));
            printf("Critical section done, restoring interrupts...\n");
            irq_restore(state);
            atomic_store(&interrupt_flag, false);
        }
        thread_yield();
    }
    return NULL;
}

int main(void)
{
    puts("RIOT interrupt handling example");

    kernel_pid_t pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                                     THREAD_CREATE_STACKTEST, thread_func, NULL, "interrupt_thread");
    if (pid <= KERNEL_PID_UNDEF) {
        puts("Error creating thread");
        return 1;
    }

    // Simulate an interrupt
    event_post(EVENT_PRIO_MEDIUM, interrupt_handler, NULL);

    return 0;
}
