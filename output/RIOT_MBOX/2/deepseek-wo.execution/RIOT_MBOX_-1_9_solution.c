
#include "evtimer_msg.h"
#include "ztimer.h"
#include "evtimer_mbox.h"
#include "mma8x5x.h"
#include "time_units.h"
#include "xtimer.h"
#include "mma8x5x_regs.h"
#include "MQTTClient.h"
#include "msg.h"
#include "timex.h"
#include "thread.h"
#include "paho_mqtt.h"
#include "mbox.h"

#define QUEUE_SIZE 8
#define STACK_SIZE 256
#define PRIORITY 5

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;
static evtimer_msg_t evtimer;

void event_handler(void *arg)
{
    (void)arg;
    msg_t msg;
    msg_receive(&msg);
    printf("Received message: %d\n", msg.content.value);
}

int main(void)
{
    // Initialize the message box
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    // Initialize the event timer
    evtimer_init_msg(&evtimer);

    // Create a thread to handle events
    thread_create(NULL, STACK_SIZE, PRIORITY, 0, event_handler, NULL, "event_handler");

    // Create an event to be sent at intervals
    evtimer_msg_event_t event;
    event.event.offset = ZTIMER_SEC(5);
    event.event.handler = event_handler;
    event.event.arg = NULL;
    event.msg.content.value = 42;

    // Add the event to the event timer
    evtimer_add_msg(&evtimer, &event, thread_getpid());

    // Main loop to keep the program running
    while (1) {
        // Do nothing, just keep the main thread alive
    }

    return 0;
}
