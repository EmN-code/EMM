
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <paho_mqtt.h>
#include <ztimer.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <mbox.h>
#include <xtimer.h>
#include <thread.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <MQTTClient.h>
#include <mma8x5x_params.h>

#define STACK_SIZE 1024
#define MSG_QUEUE_SIZE 16

static msg_t msg_queue[MSG_QUEUE_SIZE];

void *high_priority_thread(void *arg) {
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("High Priority Thread received message: %d\n", msg.content.value);
    }
    return NULL;
}

void *low_priority_thread(void *arg) {
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Low Priority Thread received message: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    pthread_t high_prio_thread, low_prio_thread;
    pthread_create(&high_prio_thread, NULL, high_priority_thread, NULL);
    pthread_create(&low_prio_thread, NULL, low_priority_thread, NULL);

    msg_t msg;
    msg.type = 1;

    for (int i = 0; i < 10; i++) {
        msg.content.value = i;
        if (i % 2 == 0) {
            msg_send(&msg, high_prio_thread);
        } else {
            msg_send(&msg, low_prio_thread);
        }
        xtimer_sleep(1);
    }

    pthread_join(high_prio_thread, NULL);
    pthread_join(low_prio_thread, NULL);

    return 0;
}
