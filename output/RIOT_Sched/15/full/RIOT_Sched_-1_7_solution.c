#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "thread_arch.h"
#include "thread_config.h"
#include "thread.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "sys/socket.h"
#include "stddef.h"
#include "sched.h"
#include "paho_mqtt.h"
#include "netinet/in.h"
#include "netemcu.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "msg.h"
#include "mutex.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "architecture.h"
#include "arpa/inet.h"
#include "stdio.h"

#define STACK_SIZE 1024
#define PRIORITY_MAIN_OF_THREAD 6
#define PRIORITY_1 5
#define PRIORITY_2 3

char stack1[STACK_SIZE];
char stack2[STACK_SIZE];

void *thread_func1(void *arg) {
    while (1) {
        printf("Thread 1 working.\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_func2(void *arg) {
    while (1) {
        printf("Thread 2 working.\n");
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread1 = thread_create(stack1, sizeof(stack1), PRIORITY_1, 0, thread_func1, NULL, "thread1");
    kernel_pid_t thread2 = thread_create(stack2, sizeof(stack2), PRIORITY_2, 0, thread_func2, NULL, "thread2");
    
    printf("Threads started.\n");

    // Change the priority of a thread at runtime
    sched_change_priority(thread_get(thread1), PRIORITY_2);

    while (1) {
        // Main work
        printf("Main thread working.\n");
        xtimer_sleep(3);
    }
    return 0;
}