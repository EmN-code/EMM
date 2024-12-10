
#include <arpa/inet.h>
#include <mma8x5x.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <stdio.h>
#include <unistd.h>
#include <MQTTClient.h>
#include <stdatomic.h>
#include <stddef.h>
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

#define STACKSIZE 1024
#define NUM_THREADS 2

void *thread_func(void *arg) {
    kernel_pid_t pid = thread_getpid();
    printf("Thread %d started\n", pid);
    while (1) {
        printf("Thread %d running\n", pid);
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    char stacks[NUM_THREADS][STACKSIZE];
    kernel_pid_t pids[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        pids[i] = thread_create(stacks[i], STACKSIZE, THREAD_PRIORITY_MAIN + 1, 0, thread_func, NULL, "thread");
        if (pids[i] <= KERNEL_PID_UNDEF) {
            printf("Error creating thread %d\n", i);
            return 1;
        }
    }

    xtimer_sleep(5);

    for (int i = 0; i < NUM_THREADS; i++) {
        sched_change_priority(thread_get(pids[i]), THREAD_PRIORITY_MAIN + 2);
        printf("Changed priority of thread %d\n", pids[i]);
    }

    return 0;
}
