#include "xtimer.h"
#include "ztimer.h"
#include "msg.h"
#include "evtimer.h"
#include "mbox.h"
#include <stdio.h>

#define TIMER_INTERVAL (1U * US_PER_SEC) // Event every 1 second

static ztimer_t my_timer;
static mbox_t my_mbox;
static msg_t msg_queue[2]; // Message queue with 2 message slots

static void timer_callback(void *arg) {
    (void)arg;
    msg_t evt_msg;
    evt_msg.type = 0x1234;  // Define a custom message type
    
    if (mbox_try_put(&my_mbox, &evt_msg) != 1) {
        puts("Failed to post message to mbox");
    }
}

int main(void) {
    puts("RIOT message box example");
    mbox_init(&my_mbox, msg_queue, 2);

    // Initialize ztimer and set a periodic timer
    ztimer_set_msg(&ztimer_sec, &my_timer, TIMER_INTERVAL, NULL, timer_callback);
    
    while (1) {
        msg_t msg;
        if (msg_receive(&msg) == 1) {
            printf("Received message with type: %x\n", msg.type);
        }
    }

    return 0;
}