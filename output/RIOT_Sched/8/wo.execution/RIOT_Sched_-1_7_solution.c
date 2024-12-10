#include <ztimer.h>
#include <thread.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sched.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <msg.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <mbox.h>
#include <timex.h>
#include <time_units.h>
#include <architecture.h>
#include <kernel_defines.h>
#include <thread_config.h>
#include <thread_arch.h>
#include <event/thread.h>

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_SECOND  (THREAD_PRIORITY_MAIN - 2)

void *thread_func(void *arg) {
    (void)arg;
    while (1) {
        // Simulate doing work
        ztimer_sleep(ZTIMER_MSEC, 1000);
        printf("Running thread with priority \n");
    }
    return NULL;
}

int main(void) {
    char stack1[THREAD_STACKSIZE_MAIN];
    char stack2[THREAD_STACKSIZE_MAIN];
    kernel_pid_t thread1_pid, thread2_pid;

    atomic_init(&thread1_pid, thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN, 0, thread_func, NULL, "thread1"));
    atomic_init(&thread2_pid, thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_SECOND, 0, thread_func, NULL, "thread2"));

    // Change priority of the first thread after creation
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_SECOND);

    // Let the main thread sleep
    ztimer_sleep(ZTIMER_MSEC, 10000);
    return 0;
}