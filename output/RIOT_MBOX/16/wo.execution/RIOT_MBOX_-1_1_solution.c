#include <ztimer.h>
#include <msg.h>
#include <event.h>
#include <stdio.h>

#define INTERVAL (1U * US_PER_SEC)
#define MESSAGE_Q_SIZE   (8)

static msg_t msg_queue[MESSAGE_Q_SIZE];

void timer_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg_send(&msg, thread_get_unchecked(pid))->message = (void *)"Event triggered!"
    printf("Timer event: %s\n", (char *)msg.content.ptr);
}

int main(void)
{
    puts("RIOT Message Box Example");

    /* Initialize message queue */
    msg_init_queue(msg_queue, MESSAGE_Q_SIZE);

    /* Start timer with callback */
    ztimer_t timer;
    ztimer_set_msg(&ztimer_periph_timer, &timer, INTERVAL, thread_get_pid());

    msg_t msg;
    while (1) {
        /* Wait for incoming messages */
        msg_receive(&msg);
        /* Process the message */
        printf("Received message: %s\n", (char *)msg.content.ptr);
    }

    return 0;
}