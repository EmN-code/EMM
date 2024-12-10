#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xtimer.h>
#include <msg.h>
#include <ztimer.h>

#define MSG_QUEUE_SIZE 8
#define SLEEP_INTERVAL (500 * US_PER_MS) // 500 ms interval

static msg_t msg_queue[MSG_QUEUE_SIZE];

void send_event(void)
{
    msg_t msg;
    msg.content.value = 1;  // Arbitrary event value
    puts("Sending event...");
    msg_send(&msg, sched_active_pid);
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    puts("Message queue initialized");

    while (1) {
        send_event();
        ztimer_sleep(ZTIMER_MSEC, SLEEP_INTERVAL);
    }

    return 0;
}