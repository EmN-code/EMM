
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "stdio.h"
#include "stdlib.h"

#define STACK_SIZE 512
#define PRIORITY   5

static char stack[STACK_SIZE];
static kernel_pid_t periodic_thread_pid;

void periodic_callback(void)
{
    printf("Periodic callback triggered!\n");
}

void* periodic_thread(void* arg)
{
    (void)arg;
    while (1) {
        periodic_callback();
        xtimer_sleep(1); // Sleep for 1 second
    }
    return NULL;
}

int main(void)
{
    // Create the periodic thread
    periodic_thread_pid = thread_create(stack, sizeof(stack), PRIORITY, 0,
                                        (thread_task_func_t)periodic_thread, NULL, "periodic_thread");

    if (periodic_thread_pid <= 0) {
        printf("Failed to create periodic thread\n");
        return 1;
    }

    printf("Periodic thread started with PID: %d\n", periodic_thread_pid);

    // Main thread can continue with other tasks or simply wait
    while (1) {
        xtimer_sleep(1); // Sleep for 1 second
    }

    return 0;
}
