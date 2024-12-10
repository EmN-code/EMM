
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <time_units.h>
#include <kernel_defines.h>
#include <thread.h>
#include <sched.h>
#include <event/thread.h>
#include <mutex.h>
#include <msg.h>
#include <mbox.h>
#include <xtimer.h>
#include <ztimer.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>

#define THREAD_STACKSIZE_DEFAULT 512

void thread_func_1(void *arg) {
    (void)arg;
    printf("Thread 1 running at priority %d\n", thread_get_priority(thread_get_active()));
    while (1) {
        // Thread 1 logic
        xtimer_sleep(1);
    }
}

void thread_func_2(void *arg) {
    (void)arg;
    printf("Thread 2 running at priority %d\n", thread_get_priority(thread_get_active()));
    while (1) {
        // Thread 2 logic
        xtimer_sleep(1);
    }
}

int main(void) {
    char stack_1[THREAD_STACKSIZE_DEFAULT];
    char stack_2[THREAD_STACKSIZE_DEFAULT];

    kernel_pid_t pid_1 = thread_create(stack_1, sizeof(stack_1), THREAD_PRIORITY_MAIN + 1, 0, thread_func_1, NULL, "thread_1");
    kernel_pid_t pid_2 = thread_create(stack_2, sizeof(stack_2), THREAD_PRIORITY_MAIN + 2, 0, thread_func_2, NULL, "thread_2");

    // Change priority of thread 1 to a lower priority
    sched_change_priority(thread_get(pid_1), THREAD_PRIORITY_MAIN + 3);
    printf("Changed priority of thread 1 to %d\n", thread_get_priority(thread_get(pid_1)));

    // Change priority of thread 2 to a higher priority
    sched_change_priority(thread_get(pid_2), THREAD_PRIORITY_MAIN);
    printf("Changed priority of thread 2 to %d\n", thread_get_priority(thread_get(pid_2)));

    return 0;
}
