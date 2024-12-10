
#include "xtimer.h"
#include "timex.h"
#include "ztimer.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval
#define LED_PIN 5                        // Assuming LED is connected to GPIO 5

static evtimer_msg_t evtimer;
static msg_t msg_queue[8];

void led_toggle(void)
{
    // Dummy function to simulate LED toggle
    printf("LED toggled\n");
}

void timer_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 0;  // Dummy value
    msg_send(&msg, thread_getpid());
}

int main(void)
{
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    evtimer_init_msg(&evtimer);

    evtimer_msg_event_t event;
    event.event.callback = timer_callback;
    event.event.arg = NULL;
    event.msg.sender_pid = thread_getpid();

    evtimer_add_msg(&evtimer, &event, thread_getpid());

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        led_toggle();
    }

    return 0;
}
