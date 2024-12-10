#include "msg.h"
#include "thread_arch.h"
#include "time_units.h"
#include "thread.h"
#include "arpa/inet.h"
#include "stdbool.h"
#include "kernel_defines.h"
#include "net/sock/udp.h"
#include "stdio.h"
#include "MQTTClient.h"
#include "evtimer_msg.h"
#include "sys/socket.h"
#include "net/sock/util.h"
#include "xtimer.h"
#include "net/gcoap.h"
#include "paho_mqtt.h"
#include "ztimer.h"
#include "architecture.h"
#include "stdlib.h"
#include "stddef.h"
#include "mbox.h"
#include "event/thread.h"
#include "net/emcute.h"
#include "timex.h"
#include "netinet/in.h"
#include "sched.h"
#include "unistd.h"
#include "evtimer_mbox.h"
#include "esp_system.h"
#include "stdatomic.h"
#include "thread_config.h"

#define THREAD_STACKSIZE_MAIN 512
#define THREAD_PRIORITY_MAIN  (THREAD_PRIORITY_MAIN - 1)

char thread_stack_one[THREAD_STACKSIZE_MAIN];
char thread_stack_two[THREAD_STACKSIZE_MAIN];

void *thread_one(void *arg) {
    while (1) {
        printf("This is thread one with priority %u\n", thread_get_priority(thread_getpid()));
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_two(void *arg) {
    while (1) {
        printf("This is thread two with priority %u\n", thread_get_priority(thread_getpid()));
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid_one = thread_create(thread_stack_one, sizeof(thread_stack_one), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, thread_one, NULL, "thread_one");
    kernel_pid_t pid_two = thread_create(thread_stack_two, sizeof(thread_stack_two), THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST, thread_two, NULL, "thread_two");

    xtimer_sleep(5);
    printf("Changing priority of threads...\n");
    sched_change_priority(thread_get(pid_one), THREAD_PRIORITY_MAIN - 3);
    sched_change_priority(thread_get(pid_two), THREAD_PRIORITY_MAIN - 1);

    while (1) {
        thread_yield();
    }

    return 0;
}