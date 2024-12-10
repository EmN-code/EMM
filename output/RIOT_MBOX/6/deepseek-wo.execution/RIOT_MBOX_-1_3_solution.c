
#include <stdio.h>
#include <unistd.h>
#include "mbox.h"
#include "ztimer.h"
#include "xtimer.h"
#include "msg.h"
#include "timex.h"
#include "event/thread.h"
#include "MQTTClient.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "time_units.h"
#include "thread.h"

#define QUEUE_SIZE 8
#define INTERVAL_MS 1000

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void) {
    msg_t msg;
    msg.type = 1;
    msg.content.value = 0;

    while (1) {
        msg.content.value++;
        mbox_put(&mbox, &msg);
        ztimer_sleep(ZTIMER_MSEC, INTERVAL_MS);
    }
}

int main(void) {
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    thread_t *sender = thread_create(
        thread_stack, sizeof(thread_stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        sender_thread, NULL, "sender");

    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg, 1);
        printf("Received message with value: %u\n", msg.content.value);
    }

    return 0;
}
