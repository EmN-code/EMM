
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <paho_mqtt.h>
#include <ztimer.h>
#include <stddef.h>
#include <msg.h>
#include <dht.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <dht_params.h>
#include <mbox.h>
#include <xtimer.h>
#include <thread.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <MQTTClient.h>

#define STACKSIZE 1024
#define MSG_QUEUE_SIZE 16

msg_t msg_queue[MSG_QUEUE_SIZE];

void high_priority_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("High Priority Thread Received: %d\n", msg.content.value);
    }
}

void low_priority_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Low Priority Thread Received: %d\n", msg.content.value);
    }
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    kernel_pid_t high_pid = thread_create(NULL, STACKSIZE, THREAD_PRIORITY_MAIN - 1, 0, high_priority_thread, NULL, "high_priority");
    kernel_pid_t low_pid = thread_create(NULL, STACKSIZE, THREAD_PRIORITY_MAIN - 2, 0, low_priority_thread, NULL, "low_priority");

    msg_t msg;
    for (int i = 0; i < 10; i++) {
        msg.content.value = i;
        if (i % 2 == 0) {
            msg_send(&msg, high_pid);
        } else {
            msg_send(&msg, low_pid);
        }
        xtimer_sleep(1);
    }

    return 0;
}
