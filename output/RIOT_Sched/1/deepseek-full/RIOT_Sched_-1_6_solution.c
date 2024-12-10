
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <net/emcute.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <arpa/inet.h>
#include <net/gcoap.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <net/sock/udp.h>

#define STACKSIZE 1024
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

void *thread_func_1(void *arg) {
    while (1) {
        // Thread 1 logic
        // For example, you can add a delay here to simulate work
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_func_2(void *arg) {
    while (1) {
        // Thread 2 logic
        // For example, you can add a delay here to simulate work
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    char stack_1[STACKSIZE];
    char stack_2[STACKSIZE];

    kernel_pid_t pid_1 = thread_create(stack_1, sizeof(stack_1), PRIORITY_HIGH, 0, thread_func_1, NULL, "thread_1");
    kernel_pid_t pid_2 = thread_create(stack_2, sizeof(stack_2), PRIORITY_LOW, 0, thread_func_2, NULL, "thread_2");

    // Change priority of thread 1 to low after some time
    xtimer_sleep(5);
    sched_change_priority(thread_get(pid_1), PRIORITY_LOW);

    // Change priority of thread 2 to high after some time
    xtimer_sleep(5);
    sched_change_priority(thread_get(pid_2), PRIORITY_HIGH);

    return 0;
}
