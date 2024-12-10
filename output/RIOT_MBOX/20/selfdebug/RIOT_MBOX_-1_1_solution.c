#include "msg.h"
#include "event.h"
#include "evtimer.h"
#include "board.h"
#include "stdio.h"
#include "xtimer.h"

#define MSG_TYPE_EVENT (0x01)
#define INTERVAL (1U * US_PER_SEC) // Time interval for sending messages

static msg_t msg_queue[8];

static void event_loop(void) {
    evtimer_t timer;
    evtimer_event_t event;
    msg_t msg;

    // Initialize event timer on the main thread
    evtimer_init_msg(&timer, (evtimer_msg_event_t *)&event, thread_getpid(), MSG_TYPE_EVENT);

    // Set timer event to trigger interval
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        // Calculate next wakeup time
        xtimer_ticks32_t wakeup_time = xtimer_now().ticks32 + INTERVAL;
        xtimer_usleep_until(&wakeup_time);

        // Send an event message
        msg_send(&msg, thread_getpid());
        printf("Message sent to the mailbox.\n");
    }
}

int main(void) {
    msg_init_queue(msg_queue, ARRAY_SIZE(msg_queue));
    puts("Message box example started.\n");
    
    event_loop();
    return 0;
}