#include <xtimer.h>
#include <evtimer.h>
#include <stdio.h>

#define INTERVAL (1U * US_PER_SEC) // 1 second interval

static evtimer_t evtimer;
static evtimer_msg_t callback_msg;

void event_callback(void *arg)
{
    (void)arg;
    printf("Event triggered!\n");
}

int main(void)
{
    // Initialize event timer
    evtimer_init(&evtimer, event_callback, NULL);
    printf("Event timer initialized\n");

    // Set periodic timer
    evtimer_set(&evtimer, INTERVAL, EVTIMER_FLAG_MSG_READY);
    printf("Event timer set to %u microseconds\n", INTERVAL);

    while (1) {
        xtimer_sleep(2); // Sleep for 2 seconds to simulate periodic activity
    }

    return 0;
}