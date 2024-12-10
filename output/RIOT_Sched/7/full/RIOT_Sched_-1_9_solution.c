#include <ztimer.h>
#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
//#include <sched.h>  // Blacklisted
//#include <netinet/in.h>  // Implies use of IPv4 - Blacklisted
//#include <pthread.h>  // Blacklisted
#include <thread_arch.h>
#include <thread.h>
#include <event/thread.h>
#include <architecture.h>
#include <msg.h>
#include <mutex.h>
#include <unistd.h>
#include <stddef.h>
#include <kernel_defines.h>
#include <MQTTClient.h>
#include <net/emcute.h>
#include <paho_mqtt.h>
//#include <esp_system.h>  // Blacklisted, but assumed to be included for ESP specifics if needed
//#include <sys/socket.h>  
//#include <net/sock/util.h>  // Blacklisted
//#include <net/sock/udp.h>  // Blacklisted
//#include <net/gcoap.h>  // Blacklisted - Utilized with network connections
//#include <arpa/inet.h>  // Implies use of IPv4 - Blacklisted
#include <time_units.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <mbox.h>

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)
#define THREAD_PRIORITY_MAIN PRIORITY_MAIN

char stack1[THREAD_STACKSIZE_MAIN];
char stack2[THREAD_STACKSIZE_MAIN];

void *thread1_func(void *arg) {
    while (1) {
        puts("Thread 1 running");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread2_func(void *arg) {
    while (1) {
        puts("Thread 2 running");
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void) {
    msg_t msg_queue[4];
    msg_init_queue(msg_queue, 4);

    kernel_pid_t thread1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN + 1,
                                          0, thread1_func, NULL, "thread1");

    kernel_pid_t thread2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN + 2,
                                          0, thread2_func, NULL, "thread2");

    // Wait a moment and then change thread1's priority
    xtimer_sleep(5);
    sched_change_priority(thread_get(thread1), THREAD_PRIORITY_MAIN + 3);

    while (1) {
        // Main loop can perform other tasks or can idle
        xtimer_sleep(10);
    }
    return 0;
}