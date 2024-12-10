
#include <arpa/inet.h>
#include <mma8x5x.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <stdio.h>
#include <unistd.h>
#include <MQTTClient.h>
#include <stddef.h>
#include <stdatomic.h>
#include <sched.h>
#include <paho_mqtt.h>
#include <stdlib.h>
#include <event/thread.h>
#include <mma8x5x_params.h>
#include <dht_params.h>
#include <esp_system.h>
#include <ztimer.h>
#include <netinet/in.h>
#include <esp_wifi.h>
#include <xtimer.h>
#include <msg.h>
#include <timex.h>
#include <net/sock/udp.h>
#include <evtimer_mbox.h>
#include <mutex.h>
#include <thread.h>
#include <mbox.h>
#include <sys/socket.h>
#include <mma8x5x_regs.h>
#include <dht.h>
#include <stdbool.h>
#include <evtimer_msg.h>
#include <net/sock/util.h>
#include <time_units.h>
#include <kernel_defines.h>

#define STACKSIZE_DEFAULT (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY_DEFAULT (THREAD_PRIORITY_MAIN)

void thread_func_1(void *arg) {
    (void)arg;
    printf("Thread 1 running\n");
    while (1) {
        // Thread 1 logic
        xtimer_sleep(1);
    }
}

void thread_func_2(void *arg) {
    (void)arg;
    printf("Thread 2 running\n");
    while (1) {
        // Thread 2 logic
        xtimer_sleep(2);
    }
}

int main(void) {
    char stack_1[STACKSIZE_DEFAULT];
    char stack_2[STACKSIZE_DEFAULT];

    kernel_pid_t pid_1 = thread_create(stack_1, sizeof(stack_1), PRIORITY_DEFAULT, 0, thread_func_1, NULL, "thread_1");
    kernel_pid_t pid_2 = thread_create(stack_2, sizeof(stack_2), PRIORITY_DEFAULT, 0, thread_func_2, NULL, "thread_2");

    // Change priority of thread 1
    sched_change_priority(thread_get(pid_1), THREAD_PRIORITY_MAIN + 1);

    // Change priority of thread 2
    sched_change_priority(thread_get(pid_2), THREAD_PRIORITY_MAIN + 2);

    printf("Main thread running\n");
    while (1) {
        // Main thread logic
        xtimer_sleep(5);
    }

    return 0;
}
