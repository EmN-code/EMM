
#include <stdio.h>
#include <thread.h>
#include <xtimer.h>
#include <time_units.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <msg.h>

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char thread_stack[THREAD_STACKSIZE];
static kernel_pid_t periodic_thread_pid;

void periodic_callback(void)
{
    printf("Periodic callback triggered!\n");
}

void* periodic_thread(void* arg)
{
    (void)arg;
    while (true) {
        periodic_callback();
        xtimer_sleep(1); // Sleep for 1 second
    }
    return NULL;
}

int main(void)
{
    periodic_thread_pid = thread_create(thread_stack, sizeof(thread_stack),
                                        THREAD_PRIORITY, 0,
                                        periodic_thread, NULL, "periodic_thread");

    if (periodic_thread_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create periodic thread\n");
        return 1;
    }

    printf("Periodic thread started\n");

    return 0;
}
