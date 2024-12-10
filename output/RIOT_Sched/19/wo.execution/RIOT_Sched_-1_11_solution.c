#include <ztimer.h>
#include <msg.h>
#include <thread_arch.h>
#include <time_units.h>
#include <thread.h>
#include <arpa/inet.h>
#include <mutex.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <net/sock/udp.h>
#include <stdio.h>
#include <MQTTClient.h>
#include <evtimer_msg.h>
#include <sys/socket.h>
#include <net/sock/util.h>
#include <xtimer.h>
#include <net/gcoap.h>
#include <paho_mqtt.h>
#include <architecture.h>
#include <stdlib.h>
#include <stddef.h>
#include <mbox.h>
#include <event/thread.h>
#include <net/emcute.h>
#include <timex.h>
#include <netinet/in.h>
#include <sched.h>
#include <esp_wifi.h>
#include <evtimer_mbox.h>
#include <esp_system.h>
#include <stdatomic.h>
#include <thread_config.h>

#define THREAD_STACKSIZE_DEFAULT (KERNEL_CONF_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)
static char stack1[THREAD_STACKSIZE_DEFAULT];
static char stack2[THREAD_STACKSIZE_DEFAULT];

void *thread_one(void *arg) {
    (void)arg;
    printf("Thread One running\n");
    while (1) {
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_two(void *arg) {
    (void)arg;
    printf("Thread Two running\n");
    while (1) {
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread_one_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 0, thread_one, NULL, "thread_one");
    kernel_pid_t thread_two_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 1, 0, thread_two, NULL, "thread_two");

    xtimer_sleep(5);
    printf("Changing priorities\n");

    /* Change the priority of the threads */
    sched_change_priority(thread_get(thread_one_pid), THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_get(thread_two_pid), THREAD_PRIORITY_MAIN - 3);

    while (1) {
        xtimer_sleep(2);
    }

    return 0;
}
