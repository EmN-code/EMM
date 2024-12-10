#include <xtimer.h>
#include <unistd.h>
#include <time_units.h>
#include <thread_config.h>
#include <thread.h>
#include <timex.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stddef.h>
#include <sched.h>
#include <paho_mqtt.h>
#include <netinet/in.h>
#include <msg.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <arpa/inet.h>
#include <MQTTClient.h>

static char thread_stack1[KERNEL_CONF_STACKSIZE_MAIN];
static char thread_stack2[KERNEL_CONF_STACKSIZE_MAIN];

void *thread_func(void *arg) {
    (void)arg;
    while (1) {
        printf("Running thread with priority %u\n", thread_get_priority(thread_getpid_of(thread_get(thread_getpid()))));
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread1 = thread_create(thread_stack1, sizeof(thread_stack1), THREAD_PRIORITY_MAIN - 1,
                                          THREAD_CREATE_STACKTEST, thread_func, NULL, "thread1");

    kernel_pid_t thread2 = thread_create(thread_stack2, sizeof(thread_stack2), THREAD_PRIORITY_MAIN - 2,
                                          THREAD_CREATE_STACKTEST, thread_func, NULL, "thread2");

    printf("Main thread waiting before changing priority\n");
    xtimer_sleep(5);

    printf("Changing priority of the threads\n");
    sched_change_priority(thread_get(thread1), THREAD_PRIORITY_MAIN - 3);
    sched_change_priority(thread_get(thread2), THREAD_PRIORITY_MAIN - 4);

    while (1) {
        thread_yield();
    }

    return 0;
}