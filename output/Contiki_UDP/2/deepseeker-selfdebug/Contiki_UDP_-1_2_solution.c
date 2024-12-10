
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/tcp-socket.h"
#include "sys/clock.h"
#include "sys/etimer.h"
#include "sys/log.h"

#define UDP_CLIENT_PORT 12345
#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define SEND_INTERVAL (5 * CLOCK_SECOND)

static struct simple_udp_connection udp_conn;

static void udp_send_state(void) {
    static const char *states[] = {"work", "not work"};
    static int state_index = 0;

    const char *state = states[state_index];
    state_index = 1 - state_index; // Toggle between 0 and 1

    uip_ipaddr_t server_ip;
    uip_ipaddr(&server_ip, 47, 102, 103, 1);

    simple_udp_sendto(&udp_conn, state, strlen(state), &server_ip);
}

int main(void) {
    simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL, UDP_SERVER_PORT, NULL);

    while(1) {
        udp_send_state();
        clock_wait(SEND_INTERVAL);
    }

    return 0;
}
