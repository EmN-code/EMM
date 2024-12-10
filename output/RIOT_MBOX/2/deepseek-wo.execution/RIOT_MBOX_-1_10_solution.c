
#include <stdio.h>
#include <stdint.h>
#include <evtimer_msg.h>
#include <ztimer.h>
#include <msg.h>
#include <timex.h>
#include <thread.h>
#include <paho_mqtt.h>
#include <mbox.h>

#define QUEUE_SIZE 16
#define STACK_SIZE 256
#define PRIORITY  5

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;
static evtimer_msg_t evtimer;

void event_handler(void *arg)
{
    msg_t msg;
    msg_receive(&msg);
    printf("Received message: %d\n", (int)msg.content.value);
}

void sender_thread(void *arg)
{
    evtimer_msg_event_t event;
    event.event.handler = event_handler;
    event.event.arg = NULL;

    while (1) {
        ztimer_sleep(ZTIMER_SEC, 2);
        evtimer_add_msg(&evtimer, &event, thread_getpid());
    }
}

int main(void)
{
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);
    evtimer_init_msg(&evtimer);

    char stack[STACK_SIZE];
    thread_create(stack, STACK_SIZE, PRIORITY, 0, sender_thread, NULL, "sender");

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Main thread received message: %d\n", (int)msg.content.value);
    }

    return 0;
}
