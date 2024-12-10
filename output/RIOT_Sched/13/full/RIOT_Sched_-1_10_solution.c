#include "xtimer.h"
#include "ztimer.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdio.h"
#include "architecture.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "msg.h"
#include "mbox.h"
#include "MQTTClient.h"
#include "kernel_defines.h"
#include "timex.h"
#include "sys/socket.h"
#include "unistd.h"
#include "arpa/inet.h"
#include "thread_arch.h"
#include "thread_config.h"
#include "thread.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "netinet/in.h"
#include "mutex.h"
#include "time_units.h"
#include "paho_mqtt.h"

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)

static char stack_one[THREAD_STACKSIZE_MAIN];
static char stack_two[THREAD_STACKSIZE_MAIN];

void *thread_func_one(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread One running\n");
        sleep(1);
    }
    return NULL;
}

void *thread_func_two(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread Two running\n");
        sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread_one = thread_create(stack_one, sizeof(stack_one), 1, THREAD_CREATE_STACKTEST, thread_func_one, NULL, "thread_one");
    if (thread_one <= KERNEL_PID_UNDEF) {
        printf("Error creating thread one\n");
    }

    kernel_pid_t thread_two = thread_create(stack_two, sizeof(stack_two), 2, THREAD_CREATE_STACKTEST, thread_func_two, NULL, "thread_two");
    if (thread_two <= KERNEL_PID_UNDEF) {
        printf("Error creating thread two\n");
    }

    // Change priority at runtime
    while (1) {
        thread_set_priority(thread_one, 3);
        thread_set_priority(thread_two, 2);
        sleep(5);
        thread_set_priority(thread_one, 2);
        thread_set_priority(thread_two, 3);
        sleep(5);
    }

    return 0;
}