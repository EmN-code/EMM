#include <ztimer.h>
#include <stdlib.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdatomic.h>
#include <thread.h>
#include <msg.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <mbox.h>
#include <mma8x5x.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <esp_wifi.h>
#include <timex.h>
#include <dht.h>
#include <dht_params.h>
#include <kernel_defines.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <esp_system.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time_units.h>
#include <mutex.h>

#define THREAD_STACKSIZE (1024)
#define HIGH_PRIORITY    (THREAD_PRIORITY_MAIN - 1)
#define MEDIUM_PRIORITY  (THREAD_PRIORITY_MAIN)
#define LOW_PRIORITY     (THREAD_PRIORITY_MAIN + 1)

char stack_high[THREAD_STACKSIZE];
char stack_medium[THREAD_STACKSIZE];

msg_t msg_queue[8];

void *high_priority_task(void *arg) {
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("High priority received: %x\n", msg.content.value);
    }
    return NULL;
}

void *medium_priority_task(void *arg) {
    msg_t msg;
    msg.content.value = 0xEF;
    while (1) {
        msg_try_send(&msg, thread_getpid_of(&high_task));
        printf("Medium priority sent: %x\n", msg.content.value);
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t high_pid = thread_create(stack_high, sizeof(stack_high),
                                          HIGH_PRIORITY, 0, high_priority_task,
                                          NULL, "high");

    kernel_pid_t medium_pid = thread_create(stack_medium, sizeof(stack_medium),
                                            MEDIUM_PRIORITY, 0, medium_priority_task,
                                            NULL, "medium");

    if (msg_init_queue(msg_queue, 8) != 1) {
        fprintf(stderr, "Failed to initialize message queue\n");
        return 1;
    }

    while (1) {
        // The main thread can also perform tasks and checks
        printf("Main thread running\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return 0;
}