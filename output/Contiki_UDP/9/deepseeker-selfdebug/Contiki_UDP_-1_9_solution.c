
#include <stdio.h>
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/tcpip.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "xtimer.h"

#define UDP_CLIENT_PORT 12345
#define UDP_SERVER_PORT 20001
#define UDP_SERVER_IP "47.102.103.1"

static struct simple_udp_connection udp_conn;

static void udp_rx_callback(struct simple_udp_connection *c,
                            const uip_ipaddr_t *sender_addr,
                            uint16_t sender_port,
                            const uip_ipaddr_t *receiver_addr,
                            uint16_t receiver_port,
                            const uint8_t *data,
                            uint16_t data_len)
{
    printf("Received response: %s\n", (char *)data);
}

static void send_device_state(void)
{
    static const char *states[] = {"work", "not work"};
    static uint8_t state_index = 0;
    char buf[32];

    snprintf(buf, sizeof(buf), "Device state: %s", states[state_index]);
    simple_udp_sendto(&udp_conn, buf, strlen(buf), NULL);

    state_index = (state_index + 1) % 2;
}

int main(void)
{
    uip_ipaddr_t server_addr;

    // Initialize the RIOT kernel
    xtimer_sleep(1);

    // Set up the UDP connection
    simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL, UDP_SERVER_PORT, udp_rx_callback);

    // Convert the server IP address to a uip_ipaddr_t
    uiplib_ipaddr_create(&server_addr, (uint8_t *)UDP_SERVER_IP, strlen(UDP_SERVER_IP));

    while (1) {
        send_device_state();
        xtimer_sleep(5);
    }

    return 0;
}
