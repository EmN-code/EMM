#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "thread_arch.h"
#include "thread.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "sched.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "mutex.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "architecture.h"
#include "arpa/inet.h"
#include "sys/socket.h"
#include "stdatomic.h"
#include "unistd.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "netinet/in.h"

void *thread_function_one(void *arg) {
    (void)arg;
    while (true) {
        puts("Thread 1 running");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_function_two(void *arg) {
    (void)arg;
    while (true) {
        puts("Thread 2 running");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    // Thread stacks
    char stack_one[THREAD_STACKSIZE_DEFAULT];
    char stack_two[THREAD_STACKSIZE_DEFAULT];

    // Creating first thread
    kernel_pid_t thread_one_pid = thread_create(stack_one, sizeof(stack_one), 8, THREAD_CREATE_STACKTEST, thread_function_one, NULL, "thread_one");

    // Creating second thread
    kernel_pid_t thread_two_pid = thread_create(stack_two, sizeof(stack_two), 10, THREAD_CREATE_STACKTEST, thread_function_two, NULL, "thread_two");

    // Changing priority of the second thread at runtime 
    sched_change_priority(thread_get(thread_two_pid), 5);

    puts("Threads are running...");

    while (true) {
        xtimer_sleep(10); // Main thread sleeps
    }
}
