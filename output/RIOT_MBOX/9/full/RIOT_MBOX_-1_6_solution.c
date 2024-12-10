#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <msg.h>
#include <net/sock/udp.h>
#include <evtimer_mbox.h>
#include <arpa/inet.h>
#include <MQTTClient.h>

#define QUEUE_SIZE     (8)
#define INTERVAL       (1000000U) // 1 second in microseconds

static msg_t queue[QUEUE_SIZE];
static mbox_t mbox;
static evtimer_t evtimer;
static evtimer_mbox_event_t event;

void *event_sender(void *arg) {
    (void)arg;
    msg_t msg;
    evtimer_init_msg(&evtimer);
    evtimer_add_mbox(&evtimer, &event, &mbox);

    while (1) {
        msg_send(&msg, mbox);
        printf("Event sent!\n");
        xtimer_usleep(INTERVAL);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(queue, QUEUE_SIZE);
    mbox_init(&mbox, queue, QUEUE_SIZE);

    // Create a separate thread for sending events
    event.thread = thread_create(msg, sizeof(msg), EVENT_PRIO, 
                                  THREAD_CREATE_STACKTEST, event_sender, 
                                  NULL, "event_sender");

    printf("System initialized
");

    return 0;
}