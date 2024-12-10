#include "ztimer.h"
#include "xtimer.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdatomic.h"
#include "thread_config.h"
#include "thread_arch.h"
#include "thread.h"
#include "time_units.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "stddef.h"
#include "stdio.h"
#include "msg.h"
#include "mutex.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "kernel_defines.h"
#include "freertos/task.h"
#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "arpa/inet.h"
#include "assert.h"
#include "architecture.h"
#include "sched.h"
#include "event/thread.h"
#include "mbox.h"
#include <unistd.h>

#define THREAD_STACKSIZE (THREAD_STACKSIZE_MAIN)

char stack_one[THREAD_STACKSIZE];
char stack_two[THREAD_STACKSIZE];
kernel_pid_t t_one_pid;
kernel_pid_t t_two_pid;

// Task function for the threads
void* thread_one(void *arg) {
    while (1) {
        puts("Thread One is running");
        ztimer_sleep(ZTIMER_SEC, 2);
    }
    return NULL;
}

void* thread_two(void *arg) {
    while (1) {
        puts("Thread Two is running");
        ztimer_sleep(ZTIMER_SEC, 2);
    }
    return NULL;
}

int main(void) {
    puts("Starting the RIOT threads example");
    
    t_one_pid = thread_create(stack_one, sizeof(stack_one),
                              THREAD_PRIORITY_MAIN - 1,
                              THREAD_CREATE_STACKTEST | THREAD_CREATE_SLEEPING,
                              thread_one, NULL, "thread_one");
    
    t_two_pid = thread_create(stack_two, sizeof(stack_two),
                              THREAD_PRIORITY_MAIN - 1,
                              THREAD_CREATE_STACKTEST | THREAD_CREATE_SLEEPING,
                              thread_two, NULL, "thread_two");

    // Wake up threads after creation
    thread_wakeup(t_one_pid);
    thread_wakeup(t_two_pid);

    // Allow threads to run initially
    ztimer_sleep(ZTIMER_SEC, 5);

    // Change the priority of thread one
    printf("Changing priority of thread one\n");
    sched_change_priority(thread_get(t_one_pid), THREAD_PRIORITY_MAIN - 2);

    // Allow threads to run with new priorities
    ztimer_sleep(ZTIMER_SEC, 5);

    return 0;
}