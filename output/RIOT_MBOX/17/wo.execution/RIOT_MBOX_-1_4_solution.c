#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdio.h"
#include "stdbool.h"
#include "paho_mqtt.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "event/thread.h"
#include "MQTTClient.h"

#define MSG_QUEUE_SIZE (8)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static msg_t _msg_queue[MSG_QUEUE_SIZE];
static char thread_stack[THREAD_STACKSIZE];

static void *msg_sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    ztimer_sleep(ZTIMER_MSEC, 1000);
    printf("Sending message from thread\n");
    msg_send(&msg, thread_getpid());
    return NULL;
}

int main(void) {
    msg_init_queue(_msg_queue, MSG_QUEUE_SIZE);
    printf("Initializing message queue\n");
    thread_create(thread_stack, THREAD_STACKSIZE, THREAD_PRIORITY, 0,
                  msg_sender_thread, NULL, "msg_sender");
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Message received - Type: %d\n", msg.type);
    }
    return 0;
}