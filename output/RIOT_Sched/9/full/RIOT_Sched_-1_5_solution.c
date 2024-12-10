#include <ztimer.h>
#include <xtimer.h>
#include <thread.h>
#include <thread_config.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <msg.h>
#include <stdbool.h>
#include <net/emcute.h>
#include <MQTTClient.h>
#include <esp_system.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <stdatomic.h>
#include <event/thread.h>
#include <dht_params.h>
#include <mutex.h>
#include <stddef.h>
#include <kernel_defines.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <net/gcoap.h>
#include <time_units.h>
#include <mbox.h>

#define STACKSIZE 1024
#define PRIORITY_MAIN 1
#define PRIORITY_OTHER 2

char stack_one[STACKSIZE];
char stack_two[STACKSIZE];

void *thread_func_one(void *arg) {
    while (1) {
        uint8_t rc;
        while(1) {
            rc = mqtt_connect(/* params */);
            if (rc != 0) {
                printf("Thread One: Connection failed. Retrying...\n");
                xtimer_sleep(1);
                continue;
            }
            printf("Thread One: Connected successfully!\n");
            break;
        }
    }
    return NULL;
}

void *thread_func_two(void *arg) {
    while (1) {
        uint8_t rc;
        while(1) {
            rc = mqtt_connect(/* params */);
            if (rc != 0) {
                printf("Thread Two: Connection failed. Retrying...\n");
                xtimer_sleep(1);
                continue;
            }
            printf("Thread Two: Connected successfully!\n");
            break;
        }
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid_one = thread_create(stack_one, sizeof(stack_one), PRIORITY_MAIN, THREAD_CREATE_STACKTEST, thread_func_one, NULL, "thread_one");
    kernel_pid_t pid_two = thread_create(stack_two, sizeof(stack_two), PRIORITY_OTHER, THREAD_CREATE_STACKTEST, thread_func_two, NULL, "thread_two");

    // Change priorities at runtime
    sched_change_priority(thread_get(pid_one), PRIORITY_OTHER);
    sched_change_priority(thread_get(pid_two), PRIORITY_MAIN);

    // Main thread loop
    while (1) {
        puts("Main thread running...");
        xtimer_sleep(1);
    }

    return 0;
}