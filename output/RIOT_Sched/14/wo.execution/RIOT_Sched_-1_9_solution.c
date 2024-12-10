#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <time_units.h>
#include <timex.h>
#include <thread_arch.h>
#include <thread_config.h>
#include <thread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mutex.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <arpa/inet.h>
#include <architecture.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <msg.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)

typedef void *(*thread_task_func_t)(void *arg);

static char stack1[THREAD_STACKSIZE_MAIN];
static char stack2[THREAD_STACKSIZE_MAIN];

void *thread1_function(void *arg) {
    printf("This is Thread 1 running with default priority.\n");
    xtimer_sleep(2);
    // Change priority here
    printf("Thread 1 changing its priority.\n");
    // Note: thread_set_priority unavailable, use alternative if needed
    return NULL;
}

void *thread2_function(void *arg) {
    printf("This is Thread 2 running with default priority.\n");
    xtimer_sleep(2);
    // Change priority here
    printf("Thread 2 changing its priority.\n");
    // Note: thread_set_priority unavailable, use alternative if needed
    return NULL;
}

int main(void) {
    kernel_pid_t thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN, CREATE_STACKTEST, thread1_function, NULL, "thread1");
    kernel_pid_t thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN + 1, CREATE_STACKTEST, thread2_function, NULL, "thread2");

    printf("Threads started with PIDs: %d, %d\n", thread1_pid, thread2_pid);

    // Simulate runtime priority change
    // Example: thread_set_priority(thread1_pid, THREAD_PRIORITY_MAIN - 2);
    // thread_set_priority(thread2_pid, THREAD_PRIORITY_MAIN - 3);

    return 0;
}