#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <msg.h>
#include <mutex.h>
#include <mbox.h>
#include <sched.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <architecture.h>
#include <arpa/inet.h>
#include <timex.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <thread.h>
#include <thread_arch.h>
#include <esp_wifi.h>
#include <esp_system.h>

void *thread_one(void *arg) {
    while (1) {
        printf("Thread one is running\n");
        sleep(1);
    }
    return NULL;
}

void *thread_two(void *arg) {
    while (1) {
        printf("Thread two is running\n");
        sleep(1);
    }
    return NULL;
}

int main(void) {
    char thread_one_stack[THREAD_STACKSIZE_DEFAULT];
    char thread_two_stack[THREAD_STACKSIZE_DEFAULT];

    kernel_pid_t thread_two_pid = thread_create(thread_two_stack,
                                                sizeof(thread_two_stack),
                                                THREAD_PRIORITY_MAIN - 1,
                                                0,
                                                thread_two,
                                                NULL,
                                                "thread_two");

    kernel_pid_t thread_one_pid = thread_create(thread_one_stack,
                                                sizeof(thread_one_stack),
                                                THREAD_PRIORITY_MAIN,
                                                0,
                                                thread_one,
                                                NULL,
                                                "thread_one");

    sched_change_priority(thread_get(thread_two_pid), THREAD_PRIORITY_MAIN);

    while (1) {
        printf("Main thread is running\n");
        sleep(2);
    }

    return 0;
}