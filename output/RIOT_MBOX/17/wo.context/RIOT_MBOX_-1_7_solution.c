#include <xtimer.h>
#include <sys/types.h>
#include <event/timeout.h>
#include <event/callback.h>
#include <kernel_defines.h>
#include <stdio.h>

#define INTERVAL (1U * US_PER_SEC)
#define QUEUE_SIZE 8

static msg_t msg_queue[QUEUE_SIZE];
static evtimer_t evtimer;
static evtimer_event_t event;
static mbox_t mbox;

static msg_t mbox_queue[QUEUE_SIZE];

static void event_callback(void *arg) {
    (void)arg;
    printf("Event triggered! Sending message to mbox.\n");
    msg_t m;
    m.content.value = 42;  // Example payload
    mbox_try_send(&mbox, &m);
}

int main(void) {
    msg_init_queue(msg_queue, QUEUE_SIZE);
    mbox_init(&mbox, mbox_queue, QUEUE_SIZE);

    evtimer_init(&evtimer, &event, 1);

    evtimer_add(&evtimer, event_callback, NULL, xtimer_now() + INTERVAL, 1);

    msg_t m;
    while (1) {
        // Wait for a message in the mailbox
        if (mbox_receive(&mbox, &m) == 1) {
            printf("Received message with value: %d\n", m.content.value);
        }
        xtimer_usleep(INTERVAL);
    }
    return 0;
}