
#include "xtimer.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/tcpip.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uip-ds6.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define SEND_INTERVAL (5 * 1000 * 1000U) // 5 seconds in microseconds

static struct simple_udp_connection udp_conn;

void send_device_state(void) {
    char buffer[] = DEVICE_STATE;
    simple_udp_sendto(&udp_conn, buffer, sizeof(buffer), SERVER_IP, SERVER_PORT);
}

int main(void) {
    // Initialize UDP connection
    simple_udp_register(&udp_conn, 0, NULL, 0, NULL, NULL);

    while (1) {
        send_device_state();
        xtimer_usleep(SEND_INTERVAL);
    }

    return 0;
}
