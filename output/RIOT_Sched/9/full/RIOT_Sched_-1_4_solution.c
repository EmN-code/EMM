#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "architecture.h"
#include "sched.h"
#include "thread.h"
#include "thread_arch.h"
#include "event/thread.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[THREAD_STACKSIZE_MAIN];

void *thread_function(void *arg) {
    (void)arg;
    while(1) {
        printf("Thread is running\n");
        thread_yield();
    }
    return NULL;
}

int main(void) {
    printf("Starting RIOT system with ESP32\n");
    kernel_pid_t pid = thread_create(stack, sizeof(stack), PRIORITY, 0, thread_function, NULL, "thread");
    if (pid == KERNEL_PID_UNDEF) {
        printf("Error creating thread\n");
        return 1;
    }

    // Simulating changing priority after creation
    printf("Changing thread priority\n");
    sched_change_priority(thread_get(pid), THREAD_PRIORITY_MAIN - 2);

    printf("Thread priority changed\n");

    while (1) {
        thread_yield();
    }
    return 0;
}