#include <ztimer.h>
#include <msg.h>
#include <stdio.h>

#define INTERVAL (1U * US_PER_SEC) // 1 second interval
#define MAIN_QUEUE_SIZE     (8)

static msg_t main_msg_queue[MAIN_QUEUE_SIZE];

void event_callback(void *arg)
{
    (void)arg;
    printf("Event triggered at time: %u\n", (unsigned)ztimer_now(ZTIMER_SEC));
}

int main(void)
{
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);

    ztimer_t timer;
    ztimer_set_msg(ZTIMER_SEC, &timer, INTERVAL, sched_active_pid, NULL);

    printf("Message box setup complete. Starting event handling.\n");

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        event_callback(NULL);
        ztimer_sleep(ZTIMER_SEC, INTERVAL);
    }

    return 0;
}