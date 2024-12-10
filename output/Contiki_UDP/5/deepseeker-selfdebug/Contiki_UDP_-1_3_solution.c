
#include <stdio.h>
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/netstack.h"
#include "xtimer.h"

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_MESSAGE "work"
#define SEND_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static void send_udp_packet(void) {
    static uint8_t payload[] = DEVICE_STATE_MESSAGE;
    static uip_ipaddr_t server_ip;
    static uip_udp_conn_t *conn;

    // Convert IP address string to uip_ipaddr_t
    if (uiplib_ipaddrconv(UDP_SERVER_IP, &server_ip) == 0) {
        puts("Error: Invalid server IP address");
        return;
    }

    // Create a new UDP connection if it doesn't exist
    if (conn == NULL) {
        conn = udp_new(NULL, 0, NULL);
        if (conn == NULL) {
            puts("Error: Failed to create UDP connection");
            return;
        }
    }

    // Set the remote IP and port
    uip_ipaddr_copy(&conn->ripaddr, &server_ip);
    conn->rport = UIP_HTONS(UDP_SERVER_PORT);

    // Send the UDP packet
    if (uip_udp_packet_sendto(conn, payload, sizeof(payload) - 1, &server_ip, UIP_HTONS(UDP_SERVER_PORT)) == 0) {
        puts("Error: Failed to send UDP packet");
    } else {
        puts("UDP packet sent successfully");
    }
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        send_udp_packet();
        xtimer_periodic_wakeup(&last_wakeup, SEND_INTERVAL);
    }

    return 0;
}
