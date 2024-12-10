#include <ztimer.h>
#include <stdio.h>
#include <evtimer.h>
#include <msg.h>

#define INTERVAL (1U * US_PER_SEC)
#define TIMER_MSG (0xAB)

static evtimer_t evtimer;
static evtimer_event_t event;

static void timer_event_handler(void *arg)
{
    (void)arg;
    printf("Periodic Event Triggered!\n");
    evtimer_add(&evtimer, &event);
}

int main(void)
{
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    uint32_t periodic_interval = INTERVAL / 1000;
    evtimer_init_msg(&evtimer, TIMER_MSG);

    event.msg.content.value = 0;
    evtimer_add(&evtimer, &event);

    while (1) {
        evtimer_add_msg(&evtimer, &event, KERNEL_PID_UNDEF);
        msg_t m;
        if (msg_receive(&m))
        {
            if (m.type == TIMER_MSG) {
                timer_event_handler(NULL);
            }
        }
    }
    return 0;
}