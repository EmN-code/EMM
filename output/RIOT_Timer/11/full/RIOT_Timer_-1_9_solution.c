#include <xtimer.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <msg.h>
#include <mutex.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <netinet/in.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <sys/socket.h>
#include <timex.h>
#include <time_units.h>
#include <unistd.h>
#include <ztimer.h>
#include <kernel_defines.h>
#include <architecture.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <stdatomic.h>
#include <arpa/inet.h>

#define TIMER_INTERVAL (1000000U) // Timer interval in microseconds (1 second)

static void _timer_callback(void *arg)
{
    (void)arg;
    printf("LED toggled\n");
}

int NetworkConnect(Network *n, char* address_ip, int port_number);

int main(void)
{
    xtimer_ticks32_t last_wakeup = xtimer_now();
    msg_t msg_q[8];
    msg_init_queue(msg_q, 8);
    evtimer_mbox_t evtimer_mbox;
    ztimer_init(ZTIMER_USEC);
    evtimer_mbox_init(&evtimer_mbox, msg_q);

    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, TIMER_INTERVAL);
        _timer_callback(NULL);

        // Attempt to connect to a network
        while (1) {
            printf("Attempting to connect to the network...\n");
            int rc = NetworkConnect(NULL, "192.168.1.10", 80);
            if (rc != 0) {
                printf("Network connection failed, retrying in 1 second...\n");
                xtimer_usleep(TIMER_INTERVAL); // Retry every second
                continue;
            } else {
                printf("Network connected successfully!\n");
                break;
            }
        }
    }

    return 0;
}