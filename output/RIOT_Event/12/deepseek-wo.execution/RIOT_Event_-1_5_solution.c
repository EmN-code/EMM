
#include <stdio.h>
#include <xtimer.h>
#include <thread.h>
#include <msg.h>

#define PERIODIC_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

kernel_pid_t periodic_thread_pid;

void periodic_callback(void)
{
    printf("Periodic callback triggered!\n");
}

void* periodic_thread(void* arg)
{
    (void)arg;

    while (1) {
        periodic_callback();
        xtimer_usleep(PERIODIC_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    periodic_thread_pid = thread_create(
        thread_stack(periodic_thread),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        periodic_thread,
        NULL,
        "periodic_thread"
    );

    if (periodic_thread_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create periodic thread\n");
        return 1;
    }

    printf("Periodic thread started\n");

    return 0;
}
