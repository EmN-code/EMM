#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <sys/socket.h>
#include <stddef.h>
#include <msg.h>
#include <mutex.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <event/thread.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <unistd.h>
#include <sched.h>
#include <thread.h>

#define MAIN_THREAD_PRIORITY    (THREAD_PRIORITY_MAIN - 1)
#define MAIN_THREAD_STACKSIZE   (THREAD_STACKSIZE_DEFAULT)

static char stack1[THREAD_STACKSIZE_MAIN];
static char stack2[THREAD_STACKSIZE_MAIN];

void thread1(void) {
    while (1) {
        printf("Thread 1 is running\n");
        ztimer_sleep(ZTIMER_SEC, 1);
    }
}

void thread2(void) {
    while (1) {
        printf("Thread 2 is running\n");
        ztimer_sleep(ZTIMER_SEC, 2);
    }
}

int main(void) {
    kernel_pid_t pid1 = thread_create(stack1, sizeof(stack1),
                                       MAIN_THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                                       thread1, NULL, "thread1");
    
    kernel_pid_t pid2 = thread_create(stack2, sizeof(stack2),
                                       MAIN_THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                                       thread2, NULL, "thread2");

    thread_wakeup(pid1);
    thread_wakeup(pid2);

    ztimer_sleep(ZTIMER_SEC, 5);

    sched_change_priority(thread_getpid_of((thread_t *)pid1), THREAD_PRIORITY_MAIN - 3);
    sched_change_priority(thread_getpid_of((thread_t *)pid2), THREAD_PRIORITY_MAIN - 4);

    while (1) {
        printf("Main thread is running\n");
        ztimer_sleep(ZTIMER_SEC, 3);
    }

    return 0;
}