#include <ztimer.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <msg.h>
#include <thread.h>
#include <sched.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <esp_system.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>

#define THREAD_STACKSIZE_MAIN   (THREAD_STACKSIZE_DEFAULT)
#define THREAD_STACKSIZE_SECOND (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_SECOND  (THREAD_PRIORITY_MAIN - 2)

char thread_stack_main[THREAD_STACKSIZE_MAIN];
char thread_stack_second[THREAD_STACKSIZE_SECOND];

static kernel_pid_t pid_main;
static kernel_pid_t pid_second;

void *thread_function_main(void *arg) {
    (void)arg;
    printf("Main Thread Running\n");
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

void *thread_function_second(void *arg) {
    (void)arg;
    printf("Second Thread Running\n");
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

int main(void) {
    /* Create the main thread */
    pid_main = thread_create(thread_stack_main, sizeof(thread_stack_main), THREAD_PRIORITY_MAIN, 0, thread_function_main, NULL, "main");

    if (pid_main <= KERNEL_PID_UNDEF) {
        printf("Error: unable to create main thread\n");
        return 1;
    }

    /* Create the second thread */
    pid_second = thread_create(thread_stack_second, sizeof(thread_stack_second), THREAD_PRIORITY_SECOND, 0, thread_function_second, NULL, "second");

    if (pid_second <= KERNEL_PID_UNDEF) {
        printf("Error: unable to create second thread\n");
        return 1;
    }

    /* Change the priority of the main thread */
    sched_change_priority(thread_get(pid_main), THREAD_PRIORITY_SECOND);
    printf("Main thread priority changed\n");

    /* Change the priority of the second thread */
    sched_change_priority(thread_get(pid_second), THREAD_PRIORITY_MAIN);
    printf("Second thread priority changed\n");

    return 0;
}