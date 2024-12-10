
#include <stdio.h>
#include "xtimer.h"
#include "net/ipv6/simple-udp.h"
#include "periph/gpio.h"

#define UDP_SERVER_ADDR "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define UDP_CLIENT_PORT 12345
#define INTERVAL_BETWEEN_SENDS (5 * 1000000U) /* 5 seconds */

static char device_state[] = "work";
static simple_udp_connection_t udp_conn;

void udp_send_state(void) {
    simple_udp_send(&udp_conn, device_state, sizeof(device_state));
}

int main(void) {
    /* Initialize UDP connection */
    simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL, UDP_SERVER_PORT, NULL);

    while (1) {
        udp_send_state();
        xtimer_usleep(INTERVAL_BETWEEN_SENDS);
    }

    return 0;
}
