#include "esp_wifi.h"
#include "esp_system.h"
#include "mutex.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "sched.h"
#include "paho_mqtt.h"
#include "timex.h"
#include "evtimer_mbox.h"
#include "kernel_defines.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "msg.h"
#include "xtimer.h"
#include "mbox.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "ztimer.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "thread.h"
#include "time_units.h"
#include "net/emcute.h"
#include "unistd.h"
#include "net/sock/udp.h"
#include "stdlib.h"
#include "evtimer_msg.h"

char stack_thread1[THREAD_STACKSIZE_DEFAULT];
char stack_thread2[THREAD_STACKSIZE_DEFAULT];

void *thread1_function(void *arg) {
    while(1) {
        xtimer_sleep(1);
        printf("Running Thread 1\n");
    }
    return NULL;
}

void *thread2_function(void *arg) {
    while(1) {
        xtimer_sleep(1);
        printf("Running Thread 2\n");
    }
    return NULL;
}

int main(void) {
    puts("RIOT Thread Priority Example\n");

    kernel_pid_t thread1 = thread_create(
        stack_thread1, sizeof(stack_thread1),
        THREAD_PRIORITY_MAIN - 2, 0,
        thread1_function, NULL, "thread1"
    );

    kernel_pid_t thread2 = thread_create(
        stack_thread2, sizeof(stack_thread2),
        THREAD_PRIORITY_MAIN - 1, 0,
        thread2_function, NULL, "thread2"
    );

    printf("Changing priority of threads\n");

    sched_change_priority(thread_get(thread1), THREAD_PRIORITY_MAIN - 1);
    sched_change_priority(thread_get(thread2), THREAD_PRIORITY_MAIN - 2);

    while (1) {
        thread_yield();
    }

    return 0;
}
