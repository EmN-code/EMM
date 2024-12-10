#include <ztimer.h>  // Import necessary for using the ztimer API
#include <timex.h>   // Import necessary for time extensions handling
#include <stdio.h>   // Standard I/O operations
#include <msg.h>     // Import necessary for message handling

#define MAIN_QUEUE_SIZE     (8)
#define TIMER_INTERVAL_SEC  (1)

static ztimer_t timer;
static msg_t msg_queue[MAIN_QUEUE_SIZE];

void timer_callback(void *arg) {
    static int event_count = 0;
    msg_t msg;
    msg.type = ++event_count;  // A simplistic way to designate the event
    msg_send(&msg, (msg_t *)arg);
    printf("Event %d sent.\n", event_count);
}

int main(void) {
    /* Initialize message queue */
    msg_init_queue(msg_queue, MAIN_QUEUE_SIZE);
    printf("Message queue initialized.\n");

    /* Set up timer */
    ztimer_set_msg(ZTIMER_MSEC, &timer, TIMER_INTERVAL_SEC * MS_PER_SEC, msg_queue);
    printf("Timer set up to send events every %d second(s).\n", TIMER_INTERVAL_SEC);

    /* Main event loop */
    msg_t m;
    while (1) {
        msg_receive(&m);  // Wait until a message is received
        printf("Event received with type: %d.\n", m.type);
        ztimer_sleep(ZTIMER_MSEC, TIMER_INTERVAL_SEC * MS_PER_SEC);  // Sleep to simulate interval timing

        // Reset timer to trigger again
        ztimer_set_msg(ZTIMER_MSEC, &timer, TIMER_INTERVAL_SEC * MS_PER_SEC, msg_queue);
    }
    return 0;
}