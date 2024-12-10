
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#include "xtimer.h"
#include "evtimer_mbox.h"
#include "thread.h"
#include "timex.h"
#include "ztimer.h"
#include "mma8x5x_regs.h"
#include "msg.h"
#include "mbox.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define STACK_SIZE 1024
#define MSG_QUEUE_SIZE 8

static char stack_high[STACK_SIZE];
static char stack_low[STACK_SIZE];

static msg_t msg_queue[MSG_QUEUE_SIZE];

void *high_priority_thread(void *arg) {
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("High Priority Thread received message: %lu\n", msg.content.value);
    }
    return NULL;
}

void *low_priority_thread(void *arg) {
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Low Priority Thread received message: %lu\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    pthread_t high_thread, low_thread;

    pthread_create(&high_thread, NULL, high_priority_thread, NULL);
    pthread_create(&low_thread, NULL, low_priority_thread, NULL);

    msg_t msg;
    msg.type = 0;

    for (uint32_t i = 0; i < 10; i++) {
        msg.content.value = i;
        if (i % 2 == 0) {
            msg_send(&msg, high_thread);
        } else {
            msg_send(&msg, low_thread);
        }
        xtimer_usleep(100000); // Sleep for 100ms
    }

    pthread_join(high_thread, NULL);
    pthread_join(low_thread, NULL);

    return 0;
}
