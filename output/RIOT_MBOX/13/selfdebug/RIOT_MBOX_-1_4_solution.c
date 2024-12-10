#include <xtimer.h>
#include <timex.h>
#include <stdio.h>
#include <msg.h>

#define INTERVAL (1U * US_PER_SEC)  // Interval of 1 second
#define MBOX_SIZE 8

static msg_t msg_queue[MBOX_SIZE];
static msg_t msg;
static xtimer_t timer;

void timer_callback(void *arg)
{
    (void)arg;
    msg.content.value = 1;
    msg_send(&msg, thread_getpid());  // Send message to own thread
    xtimer_set(&timer, INTERVAL);
}

int main(void)
{
    msg_init_queue(msg_queue, MBOX_SIZE);
    printf("Message box and timers initialized.\n");

    xtimer_set(&timer, INTERVAL);
    timer.callback = timer_callback;
    timer.arg = NULL;

    while (1) {
        msg_receive(&msg);
        printf("Received a message with value: %d\n", msg.content.value);
    }

    return 0;
}