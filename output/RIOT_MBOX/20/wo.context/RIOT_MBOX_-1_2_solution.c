#include <ztimer.h>
#include <stdio.h>
#include <msg.h>
#include <kernel.h>

#define TIMER_INTERVAL (1000000U)  // 1 second in microseconds
#define MSG_QUEUE_SIZE 8

static msg_t msg_queue[MSG_QUEUE_SIZE];
static ztimer_t timer;

void timer_callback(void *arg) {
    (void)arg; // Suppress unused variable warning
    msg_t msg;
    msg.type = 1;
    msg_send(&msg, thread_getpid());
    printf("Message sent with type: %u\n", msg.type);
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    ztimer_set(ZTIMER_SEC, &timer, TIMER_INTERVAL);
    printf("Timer set for interval: %u microseconds\n", TIMER_INTERVAL);
    while(1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Received message of type: %u\n", msg.type);
    }
    return 0;
}