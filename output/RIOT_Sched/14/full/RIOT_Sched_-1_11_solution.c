#include "xtimer.h"
#include "sched.h"
#include "netinet/in.h"
#include "stdlib.h"
#include "stdio.h"
#include "sys/socket.h"
#include "timex.h"
#include "unistd.h"
#include "esp_system.h"
#include "net/sock/udp.h"
#include "arpa/inet.h"
#include "kernel_defines.h"
#include "net/sock/util.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "event/thread.h"
#include "msg.h"
#include "paho_mqtt.h"
#include "ztimer.h"
#include "MQTTClient.h"
#include "architecture.h"
#include "mutex.h"
#include "thread_arch.h"
#include "event/thread.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "stdbool.h"
#include "evtimer_msg.h"
#include "stdatomic.h"
#include "stddef.h"

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)

static char thread_stack1[THREAD_STACKSIZE_MAIN];
static char thread_stack2[THREAD_STACKSIZE_MAIN];

char stack[THREAD_STACKSIZE_DEFAULT];

static void *thread_func1(void *arg) {
    (void)arg; // This suppresses the unused parameter warning
    while (1) {
        puts("Thread 1 running");
        xtimer_sleep(2);
    }
    return NULL;
}

static void *thread_func2(void *arg) {
    (void)arg; // This suppresses the unused parameter warning
    while (1) {
        puts("Thread 2 running");
        xtimer_sleep(3);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread1, thread2;

    thread1 = thread_create(thread_stack1, sizeof(thread_stack1), THREAD_PRIORITY_MAIN - 1,
                           THREAD_CREATE_STACKTEST, thread_func1, NULL, "thread_1");
    thread2 = thread_create(thread_stack2, sizeof(thread_stack2), THREAD_PRIORITY_MAIN - 1,
                           THREAD_CREATE_STACKTEST, thread_func2, NULL, "thread_2");

    printf("Threads created: %d, %d\n", thread1, thread2);

    // Assuming interrupts are already disabled, as required
    sched_change_priority(thread_get(thread1), THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_get(thread2), THREAD_PRIORITY_MAIN - 3);

    while (1) {
        // Main thread doing something...
        puts("Main thread");
        xtimer_sleep(1);
    }

    return 0;
}