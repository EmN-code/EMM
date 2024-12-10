#include "esp_wifi.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "sys/socket.h"
#include "stdlib.h"

#include "esp_system.h"
#include "stdbool.h"
#include "stdint.h"
#include "assert.h"

#include "evtimer_init_mbox.h"
#include "evtimer_add_mbox.h"
#include "evtimer_add_msg.h"

#define CALLBACK_INTERVAL_SEC 1

void event_periodic_callback(void)
{
    // Your callback function code here
}

int main(void)
{
    evtimer_t my_ev_timer;
    evtimer_mbox_event_t my_event;
    mbox_t my_mbox;

    evtimer_init_mbox(&my_ev_timer);
    evtimer_add_mbox(&my_ev_timer, &my_event, &my_mbox);

    while (1)
    {
        // Delay for the required periodic interval (in seconds)
        sleep(CALLBACK_INTERVAL_SEC);
        event_periodic_callback();
    }

    return 0;
}
