#include <stdio.h>
#include <msg.h>
#include <ztimer.h>

#define TIMER_INTERVAL 1000000  // Timer interval in microseconds
#define QUEUE_SIZE      8

static msg_t msg_queue[QUEUE_SIZE];
static ztimer_t timer;

void timer_callback(void *arg) {
    (void)arg;  // Unused
    msg_t msg;
    msg.content.value = 42;  // Example payload
    if (msg_send(&msg, msg_queue) < 1) {
        puts("Failed to send message");
    }
}

int main(void) {
    puts("RIOT Message Box Example");

    // Initialize message queue
    msg_init_queue(msg_queue, QUEUE_SIZE);

    // Initialize and set the timer
    ztimer_set(ZTIMER_SEC, &timer, TIMER_INTERVAL, timer_callback, NULL);

    // Receive messages
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %d\n", msg.content.value);
    }

    return 0;
}