
#include <stdio.h>
#include <stdlib.h>
#include <xtimer.h>
#include <thread.h>
#include <msg.h>

#define PERIODIC_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static void *periodic_callback(void *arg)
{
    (void)arg;

    while (1) {
        printf("Periodic callback triggered!\n");
        xtimer_usleep(PERIODIC_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    kernel_pid_t pid = thread_create(
        NULL,
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        periodic_callback,
        NULL,
        "periodic_callback"
    );

    if (pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread\n");
        return 1;
    }

    printf("Thread created with PID: %d\n", pid);

    return 0;
}
