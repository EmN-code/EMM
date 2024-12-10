#include <ztimer.h>
#include <stdio.h>
#include <msg.h>

#define INTERVAL (1U * ZTIMER_MSEC)  // send event every 1 second
#define MAIN_QUEUE_SIZE (8)

static msg_t main_msg_queue[MAIN_QUEUE_SIZE];

void sender(void)
{
    msg_t msg;
    msg.content.value = 42; // Arbitrary message content / event
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, INTERVAL);
        msg_send(&msg, sched_active_pid); // Sending message to active thread (this one)
        printf("Message sent with content: %d\n", msg.content.value);
    }
}

int main(void)
{
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);
    printf("Message box example started\n");
    sender();
    return 0;
}