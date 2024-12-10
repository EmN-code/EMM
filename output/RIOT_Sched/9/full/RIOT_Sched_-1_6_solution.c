#include <ztimer.h>
#include <ztimer.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <time_units.h>
#include <unistd.h>
#include <thread.h>
#include <thread_config.h>
#include <sched.h>
#include <msg.h>
#include <architecture.h>
#include <event/thread.h>
#include <kernel_defines.h>
#include <timex.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <dht_params.h>
#include <dht.h>
#include <mma8x5x_params.h>
#include <mutex.h>
#include <MQTTClient.h>

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)

char stack1[THREAD_STACKSIZE];
char stack2[THREAD_STACKSIZE];

kernel_pid_t thread1_pid;
kernel_pid_t thread2_pid;

void *thread_handler1(void *arg) {
    (void)arg;
    while (1) {
        // Perform some task
        ztimer_sleep(ZTIMER_MSEC, 500);
    }
    return NULL;
}

void *thread_handler2(void *arg) {
    (void)arg;
    while (1) {
        // Perform some task
        ztimer_sleep(ZTIMER_MSEC, 500);
    }
    return NULL;
}

int main(void) {
    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN + 1, CREATE_STACKTEST, thread_handler1, NULL, "thread_1");
    if (thread1_pid <= KERNEL_PID_UNDEF) {
        printf("Thread 1 creation failed\n");
        return 1;
    }

    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN + 2, CREATE_STACKTEST, thread_handler2, NULL, "thread_2");
    if (thread2_pid <= KERNEL_PID_UNDEF) {
        printf("Thread 2 creation failed\n");
        return 1;
    }

    // Modify the priority of the threads
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN + 2);
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN + 1);

    Network network;
    while (1) {
        int rc = NetworkConnect(&network, "192.168.1.1", 1883);
        if (rc != 0) {
            printf("Network connection failed, retrying...\n");
            ztimer_sleep(ZTIMER_SEC, 1);
            continue;
        } else {
            printf("Network connected successfully\n");
            break;
        }
    }

    return 0;
}
