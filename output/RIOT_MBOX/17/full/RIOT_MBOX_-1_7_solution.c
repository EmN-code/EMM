#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mutex.h"
#include "msg.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "dht_params.h"
#include "arpa/inet.h"
#include "unistd.h"
#include "thread.h"
#include "time_units.h"
#include "MQTTClient.h"

#define MAIN_QUEUE_SIZE (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static void *_message_box_thread(void *arg) {
    (void) arg;
    msg_t msg;
    mbox_t mbox;
    msg_t mbox_queue[MAIN_QUEUE_SIZE];
    mbox_init(&mbox, mbox_queue, MAIN_QUEUE_SIZE);

    while (1) {
        /* Block and wait for message */
        msg_receive(&msg);
        printf("Received message with type: %d\n", msg.type);
     
        /* Optionally, send the message to a mailbox/event timer */
        evtimer_mbox_event_t event_msg;
        evtimer_add_mbox((evtimer_t *)&mbox, &event_msg, &mbox);
    }

    return NULL;
}

int main(void) {
    puts("RIOT message box example started");
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, _message_box_thread, NULL, "message_box");

    /* Simulate sending messages at intervals */
    msg_t msg;
    msg.type = 1;
    while (1) {
        printf("Sending message\n");
        msg_send(&msg, thread_getpid());
        xtimer_sleep(5);
    }

    return 0;
}