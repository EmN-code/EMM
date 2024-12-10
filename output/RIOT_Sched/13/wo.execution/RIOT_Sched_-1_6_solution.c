#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "thread.h"
#include "sched.h"
#include "msg.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "arpa/inet.h"

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)

char thread_stack_one[THREAD_STACKSIZE_MAIN];
char thread_stack_two[THREAD_STACKSIZE_MAIN];

void *thread_one(void *arg) {
    (void)arg;
    while (1) {
        printf("Running thread one\n");
        sleep(1);
    }
    return NULL;
}

void *thread_two(void *arg) {
    (void)arg;
    while (1) {
        printf("Running thread two\n");
        sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid_one = thread_create(thread_stack_one, sizeof(thread_stack_one),
                                         THREAD_PRIORITY_MAIN - 1, 0,
                                         thread_one, NULL, "thread_one");

    kernel_pid_t pid_two = thread_create(thread_stack_two, sizeof(thread_stack_two),
                                         THREAD_PRIORITY_MAIN - 2, 0,
                                         thread_two, NULL, "thread_two");

    if (pid_one >= 0 && pid_two >= 0) {
        printf("Threads successfully created\n");
    } else {
        printf("Error creating threads\n");
    }

    sleep(3);

    sched_change_priority(thread_get(pid_one), THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_get(pid_two), THREAD_PRIORITY_MAIN - 1);

    printf("Thread priorities changed\n");

    while (1) {
        sleep(5);
    }
    return 0;
}