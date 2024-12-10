#include <ztimer.h>
#include <xtimer.h>
#include <msg.h>
#include <evtimer.h>
#include <stdio.h>

#define TIMER_INTERVAL (1U * US_PER_SEC)
#define MSG_QUEUE_SIZE (8)

static msg_t msg_queue[MSG_QUEUE_SIZE];
static ztimer_t timer;

void timer_callback(void *arg)
{
    (void)arg;
    printf("Timer event triggered\n");

    msg_t msg;
    msg.type = 0;  // Define appropriate event type
    msg.content.value = 42; // Example value
    msg_send(&msg, sched_active_pid);
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    printf("Message queue initialized\n");

    ztimer_set(ZTIMER_MSEC, &timer, TIMER_INTERVAL);
    timer.callback = timer_callback;

    while (1) {
        msg_t msg;
        msg_receive(&msg);

        if (msg.type == 0) {
            printf("Received message with value: %" PRIu32 "\n", msg.content.value);
        }
    }

    return 0;
}