
#include <stdio.h>  // Include stdio.h to resolve the implicit declaration of printf
#include "thread.h"
#include "msg.h"
#include "ztimer.h"
#include "event.h"
#include "mbox.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static mbox_t mbox;
static msg_t mbox_msgs[10];

void event_handler(event_t *event)
{
    printf("Event received\n");
}

void *event_thread(void *arg)
{
    (void)arg;
    event_queue_t queue;
    event_queue_init(&queue);

    event_t event;
    event_init(&event, event_handler);

    while (1) {
        event_post(&queue, &event);
        ztimer_sleep(ZTIMER_MSEC, 1000);  // Post event every 1 second
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    mbox_init(&mbox, mbox_msgs, 10);

    thread_t *event_thread_pid = thread_create(
        NULL,
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        event_thread,
        NULL,
        "event_thread"
    );

    (void)event_thread_pid;

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Message received\n");
    }

    return 0;
}
