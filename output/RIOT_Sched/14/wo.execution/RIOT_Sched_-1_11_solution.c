#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <kernel_defines.h>
#include <thread.h>
#include <mutex.h>
#include <msg.h>
#include <event/thread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <esp_wifi.h>
#include <net/emcute.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <esp_system.h>

#define THREAD_STACKSIZE_DEFAULT    (1024)
#define THREAD_PRIORITY_MAIN        (NODE_THREAD_PRIORITY_MAIN)
#define THREAD_CREATE_STACKTEST     0
#define THREAD_CREATE_SLEEPING      0

kernel_pid_t thread_1_pid;
char stack_thread_1[THREAD_STACKSIZE_DEFAULT];

kernel_pid_t thread_2_pid;
char stack_thread_2[THREAD_STACKSIZE_DEFAULT];

void *thread_func_1(void *arg) {
    while (1) {
        printf("Thread 1 is running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_func_2(void *arg) {
    while (1) {
        printf("Thread 2 is running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    puts("RIOT application starting.");

    /* Create thread 1 */
    thread_1_pid = thread_create(stack_thread_1, sizeof(stack_thread_1),
                                 THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                                 thread_func_1, NULL, "thread_1");
    
    /* Create thread 2 */
    thread_2_pid = thread_create(stack_thread_2, sizeof(stack_thread_2),
                                 THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST,
                                 thread_func_2, NULL, "thread_2");

    /* Simulate priority change at runtime */
    printf("Changing priorities at runtime\n");
    thread_set_priority(thread_1_pid, THREAD_PRIORITY_MAIN - 3);
    thread_set_priority(thread_2_pid, THREAD_PRIORITY_MAIN - 1);

    while (1) {
        /* Keep the main thread running */
        xtimer_sleep(10);
    }

    return 0;
}