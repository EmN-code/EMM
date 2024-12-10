
#include <stdio.h>
#include <thread.h>
#include <msg.h>
#include <mbox.h>
#include <xtimer.h>

#define MSG_QUEUE_SIZE 10
#define MBOX_SIZE 10
#define INTERVAL_MS 1000000

static mbox_t mbox;
static msg_t msg_queue[MSG_QUEUE_SIZE];
static msg_t mbox_msgs[MBOX_SIZE];

void *event_sender(void *arg) {
    (void)arg;
    msg_t msg;
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL_MS);
        msg.content.value = 1;
        mbox_put(&mbox, &msg);
    }

    return NULL;
}

void *event_receiver(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg, 1);
        printf("Received event with value: %d\n", (int)msg.content.value);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    mbox_init(&mbox, mbox_msgs, MBOX_SIZE);

    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, event_sender, NULL, "event_sender");

    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, event_receiver, NULL, "event_receiver");

    return 0;
}
