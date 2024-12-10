#include "xtimer.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "net/sock/util.h"
#include "architecture.h"
#include "netinet/in.h"
#include "sched.h"
#include "event/thread.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "mma8x5x_regs.h"
#include "stdio.h"
#include "stdlib.h"
#include "msg.h"
#include "dht_params.h"
#include "thread_config.h"
#include "mma8x5x_params.h"
#include "net/emcute.h"
#include "stddef.h"
#include "kernel_defines.h"
#include "ztimer.h"
#include "mutex.h"
#include "unistd.h"
#include "evtimer_msg.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "time_units.h"
#include "mma8x5x.h"
#include "net/sock/udp.h"
#include "thread.h"
#include "thread_arch.h"
#include "arpa/inet.h"
#include "timex.h"
#include "esp_system.h"
#include "dht.h"
#include "sys/socket.h"
#include "net/gcoap.h"

#define STACK_SIZE 1024
#define THREAD_PRIORITY_ONE (THREAD_PRIORITY_DEFAULT - 1)
#define THREAD_PRIORITY_TWO (THREAD_PRIORITY_DEFAULT - 2)

char stack_one[STACK_SIZE];
char stack_two[STACK_SIZE];

void *thread_function_one(void *arg) {
    while (1) {
        // thread work
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_function_two(void *arg) {
    while (1) {
        // thread work
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread_one_pid, thread_two_pid;

    thread_one_pid = thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY_ONE, 0, thread_function_one, NULL, "thread_one");
    thread_two_pid = thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY_TWO, 0, thread_function_two, NULL, "thread_two");

    if (thread_one_pid == -1 || thread_two_pid == -1) {
        printf("Error creating threads\n");
        return 1;
    }

    xtimer_sleep(5); // Allow threads to run for some time

    sched_change_priority(thread_get(thread_one_pid), THREAD_PRIORITY_TWO);
    sched_change_priority(thread_get(thread_two_pid), THREAD_PRIORITY_ONE);

    // Main loop
    while (1) {
        xtimer_sleep(1); // Sleep to keep the main thread alive
    }

    return 0;
}