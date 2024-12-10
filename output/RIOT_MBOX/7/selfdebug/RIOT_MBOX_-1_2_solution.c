#include <ztimer.h>
#include <msg.h>
#include <stdio.h> 
#include <xtimer.h>

#define TIMER_INTERVAL_SEC (5U)
#define MSG_QUEUE_SIZE (8)

static msg_t msg_queue[MSG_QUEUE_SIZE];

static void _timer_callback(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 0x1234;
    printf("Timer expired, sending message\n");
    msg_send(&msg, thread_getpid());
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    ztimer_t timer;
    timer.callback = _timer_callback;
    timer.arg = NULL;
    ztimer_set(ZTIMER_SEC, &timer, TIMER_INTERVAL_SEC * ZTIMER_USEC_TO_TICKS);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Received message of type: %04x\n", msg.type);
    }
    return 0;
}