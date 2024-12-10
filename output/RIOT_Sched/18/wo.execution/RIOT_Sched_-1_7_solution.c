#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <thread_helpers.h>
#include <thread.h>
#include <stdio.h>
#include <stdbool.h>
#include <mutex.h>
#include <msg.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdatomic.h>
#include <unistd.h>
#include <sched.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <sys/net/driver.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <kernel_defines.h>
#include <thread_config.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <mbox.h>

#define THREAD_STACKSIZE (THREAD_STACKSIZE_MAIN)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN-1)

static char thread_one_stack[THREAD_STACKSIZE];     // Stack for thread one
static char thread_two_stack[THREAD_STACKSIZE];     // Stack for thread two

mutex_t lock;

void *thread_one_func(void *arg) {
    (void) arg;
    while(1) {
        mutex_lock(&lock);
        printf("Thread One is Running\n");
        mutex_unlock(&lock);
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_two_func(void *arg) {
    (void) arg;
    while(1) {
        mutex_lock(&lock);
        printf("Thread Two is Running\n");
        mutex_unlock(&lock);
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread_one_pid;
    kernel_pid_t thread_two_pid;
    
    mutex_init(&lock);

    // Create thread one
    thread_one_pid = thread_create(thread_one_stack, sizeof(thread_one_stack),
                                   THREAD_PRIORITY, 0,
                                   thread_one_func, NULL, "thread_one");
    
    // Create thread two
    thread_two_pid = thread_create(thread_two_stack, sizeof(thread_two_stack),
                                   THREAD_PRIORITY - 1, 0,
                                   thread_two_func, NULL, "thread_two");

    // Change the priority of thread two
    sched_change_priority(thread_get(thread_two_pid), THREAD_PRIORITY);

    while(1) {
        // Main thread loop
        mutex_lock(&lock);
        printf("Main thread is Running\n");
        mutex_unlock(&lock);
        xtimer_sleep(3);
    }

    return 0;
}
