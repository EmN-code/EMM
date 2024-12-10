
#include "evtimer_msg.h"
#include "ztimer.h"
#include "evtimer_mbox.h"
#include "time_units.h"
#include "xtimer.h"
#include "mma8x5x_regs.h"
#include "MQTTClient.h"
#include "msg.h"
#include "timex.h"
#include "thread.h"
#include "mbox.h"

#define STACK_SIZE 256
#define PRIORITY 5
#define MSG_QUEUE_SIZE 8

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;
static evtimer_t evtimer;

void event_handler(void *arg) {
    msg_t msg;
    msg.content.ptr = (void *)arg;
    msg_send(&msg, thread_getpid());
}

int main(void) {
    // Initialize the message box
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    // Initialize the event timer
    evtimer_init_msg(&evtimer);

    // Add an event to the event timer
    evtimer_add_msg(&evtimer, (evtimer_msg_event_t){
        .event = { .handler = event_handler, .arg = (void *)1 },
        .offset = ZTIMER_SEC(5)
    });

    // Main loop to receive messages
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with content: %p\n", msg.content.ptr);
    }

    return 0;
}
