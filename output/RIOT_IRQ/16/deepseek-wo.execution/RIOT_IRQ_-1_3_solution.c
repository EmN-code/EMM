
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <msg.h>
#include <ztimer.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <xtimer.h>
#include <time_units.h>
#include <thread.h>

#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACK_SIZE];
static volatile bool interrupt_occurred = false;

void interrupt_handler(void *arg)
{
    (void)arg;
    interrupt_occurred = true;
}

void *interrupt_thread(void *arg)
{
    (void)arg;

    while (1) {
        if (interrupt_occurred) {
            printf("Interrupt occurred!\n");
            interrupt_occurred = false;
        }
        ztimer_sleep(ZTIMER_MSEC, 100);
    }

    return NULL;
}

int main(void)
{
    kernel_pid_t pid = thread_create(stack, sizeof(stack), PRIORITY, THREAD_CREATE_STACKTEST, interrupt_thread, NULL, "interrupt_thread");

    if (pid <= 0) {
        printf("Failed to create thread\n");
        return 1;
    }

    // Simulate an interrupt by setting a flag
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 500);
        interrupt_handler(NULL);
    }

    return 0;
}
