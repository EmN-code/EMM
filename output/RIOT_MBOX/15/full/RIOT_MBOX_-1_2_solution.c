#include "xtimer.h"
#include "timex.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "mbox.h"
#include "msg.h"
#include "time_units.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

#define TIMER_PERIOD (5U * US_PER_SEC) // 5 seconds

mbox_t mbox;
msg_t mbox_queue[8];

void *event_thread(void *arg) {
    (void)arg;
    evtimer_msg_event_t event;
    event.mbox = &mbox;

    puts("Event thread initialized.");

    while (1) {
        evtimer_mbox_init_msg_event(&event, &mbox, &mbox_queue[0], 8);
        evtimer_msg_event_periodic(&event, TIMER_PERIOD);
        puts("Event sent!");
        xtimer_sleep(5);  // Delay between events
    }
    return NULL;
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    evtimer_msg_event_t event;
    evtimer_mbox_init_msg_event(&event, &mbox, &mbox_queue[0], 8);
    
    puts("Starting event thread...");
    uint16_t stack_size = THREAD_STACKSIZE_DEFAULT;
    thread_create((char *)stack_size, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, 0, event_thread, NULL, "event_thread");

    return 0;
}