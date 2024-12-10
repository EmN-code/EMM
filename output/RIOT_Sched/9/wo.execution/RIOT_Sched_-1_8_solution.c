#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <sched.h>
#include <netinet/in.h>
#include <assert.h>
#include <architecture.h>
#include <event/thread.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <freertos/task.h>
#include <freertos/FreeRTOS.h>
#include <kernel_defines.h>
#include <mbox.h>
#include <msg.h>
#include <MQTTClient.h>
#include <mutex.h>
#include <net/gcoap.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <arpa/inet.h>

#include "thread_config.h"
#include "thread_arch.h"

#define STACKSIZE 2048
#define PRIO_THREAD1 (THREAD_PRIORITY_MAIN + 1)
#define PRIO_THREAD2 (THREAD_PRIORITY_MAIN + 2)
#define THREAD_NAME "Worker"

static char stack1[STACKSIZE];
static char stack2[STACKSIZE];

void *thread_handler(void *arg) {
    (void)arg;
    while(1) {
        printf("Thread running\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid1 = thread_create(stack1, sizeof(stack1), PRIO_THREAD1, THREAD_CREATE_STACKTEST, thread_handler, NULL, THREAD_NAME);
    kernel_pid_t pid2 = thread_create(stack2, sizeof(stack2), PRIO_THREAD2, THREAD_CREATE_STACKTEST, thread_handler, NULL, THREAD_NAME);

    ztimer_sleep(ZTIMER_MSEC, 3000);
    
    thread_t *t1 = thread_get(pid1);
    thread_t *t2 = thread_get(pid2);

    if (t1 != NULL && t2 != NULL) {
        sched_change_priority(t1, PRIO_THREAD2);
        sched_change_priority(t2, PRIO_THREAD1);
    }

    while(1) {
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}