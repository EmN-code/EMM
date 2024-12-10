#include <ztimer.h>
#include <msg.h>
#include <mbox.h>
#include <stdio.h>

#define MSG_QUEUE_SIZE 8

static mbox_t mbox;
static msg_t msg_queue[MSG_QUEUE_SIZE];
static ztimer_t timer;

void timer_callback(void *arg)
{
    msg_t msg;
    msg.content.value = (uint32_t)arg;
    mbox_send(&mbox, &msg);
    printf("Message sent: %lu\n", (uint32_t)arg);
    ztimer_set(&timer, 1000000UL);  // re-set timer for periodic sending
}

int main(void)
{
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    timer.callback = timer_callback;
    timer.arg = (void *)42;  // example argument

    ztimer_set(&timer, 1000000UL);  // initially set timer for periodic event

    while (1) {
        msg_t msg;
        if (mbox_receive(&mbox, &msg) == 1) {
            printf("Message received: %lu\n", msg.content.value);
        }
    }

    return 0;
}
