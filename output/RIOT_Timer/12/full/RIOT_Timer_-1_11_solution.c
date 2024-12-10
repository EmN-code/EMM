#include "xtimer.h"
#include "stdio.h"  // Needed for printf

static inline void my_timer_handler(void)
{
    // Code to turn on the LED
    printf("LED toggled\n");
}

void retry_network_connect(Network *network) {
    int rc;
    while (1) {
        rc = NetworkConnect(network, "192.168.0.1", 1883);
        if (rc != 0) {
            printf("connect failed, retrying...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("connect successfully\n");
            break;
        }
    }
}

int main(void)
{
    evtimer_t my_timer;
    evtimer_init(&my_timer, my_timer_handler);
    evtimer_add_msg(&my_timer, &(event->event), 0);
    while (1) {
        evtimer_do_work();
        xtimer_sleep(1);
    }
    return 0;
}
