#include <xtimer.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <stdatomic.h>
#include <thread_arch.h>
#include <thread.h>
#include <event/thread.h>
#include <esp_system.h>
#include <netinet/in.h>
#include <paho_mqtt.h>
#include <mutex.h>
#include <stddef.h>
#include <msg.h>
#include <kernel_defines.h>
#include <stdbool.h>
#include <mbox.h>
#include <net/sock/util.h>
#include <sys/socket.h>
#include <esp_wifi.h>
#include <time_units.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <evtimer_msg.h>
#include <architecture.h>
#include <evtimer_mbox.h>
#include <arpa/inet.h>
#include <MQTTClient.h>
#include <net/gcoap.h>
#include <timex.h>
#include <unistd.h>

#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)
#define STACK_SIZE_THREAD  (1024)

char stack1[STACK_SIZE_THREAD];
char stack2[STACK_SIZE_THREAD];

void *thread1_func(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 1 is running\n");
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread2_func(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 2 is running\n");
        xtimer_sleep(3);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid1, pid2;

    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST,
                         thread1_func, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST,
                         thread2_func, NULL, "thread2");

    if (pid1 <= 0 || pid2 <= 0) {
        printf("Error creating threads\n");
        return 1;
    }

    while (1) {
        printf("Changing priorities\n");
        sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN);
        sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN - 1);
        xtimer_sleep(5);

        sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN - 1);
        sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN);
        xtimer_sleep(5);
    }

    return 0;
}