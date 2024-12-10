
#include "ztimer.h"
#include "thread.h"
#include "msg.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "paho_mqtt.h"

#define STACKSIZE (1024)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACKSIZE];
static kernel_pid_t periodic_thread_pid;

void periodic_callback(void)
{
    printf("Periodic callback triggered!\n");
}

void* periodic_thread(void *arg)
{
    (void)arg;
    while (1) {
        periodic_callback();
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

int main(void)
{
    periodic_thread_pid = thread_create(stack, sizeof(stack), PRIORITY,
                                        THREAD_CREATE_STACKTEST, periodic_thread,
                                        NULL, "periodic_thread");

    if (periodic_thread_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create periodic thread\n");
        return 1;
    }

    printf("Periodic thread started with PID %d\n", periodic_thread_pid);

    return 0;
}
