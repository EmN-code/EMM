#include <ztimer.h>  // ZTimer provides timer functionalities
#include <msg.h>     // msg module is used for inter-thread communication
#include <evtimer.h> // evtimer provides event timer functionalities

#define INTERVAL (1000U) // Interval in milliseconds
#define MAIN_QUEUE_SIZE (8)

static msg_t main_msg_queue[MAIN_QUEUE_SIZE];

void event_handler(evtimer_event_t *event) {
    msg_t msg;
    msg_receive(&msg);
    printf("Received event: %ld\n", (long)msg.content.value);
}

int main(void) {
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);
    printf("Message Box Demo\n");

    evtimer_t evtimer;
    evtimer_msg_event_t event;
    evtimer_event_t *event_ptr = (evtimer_event_t*)&event;

    evtimer_init(&evtimer, ztimer_clock, &event_handler);
    evtimer_msg_create(&event, ztimer_now(ZTIMER_MSEC) + INTERVAL, msg_receive, NULL);
    evtimer_add(&evtimer, event_ptr);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        evtimer_msg_event_t* evt = (evtimer_msg_event_t*)msg.content.ptr;
        evt->msg.content.value = (intptr_t) msg.content.ptr;
        printf("Handled event: %ld\n", (long)evt->msg.content.value);
    }
    return 0;
}