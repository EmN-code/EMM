#include <stdio.h>
#include <string.h>
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-icmp6.h"

#define LOCAL_PORT 12345
#define TARGET_IP "47.102.103.1"
#define TARGET_PORT 20001

static struct uip_udp_conn *udp_connection;
char device_state[] = "work";

void init_udp_connection() {
    uip_ipaddr_t target_ip;
    uip_ipaddr(&target_ip, 47, 102, 103, 1);
    udp_connection = uip_udp_new(&target_ip, UIP_HTONS(TARGET_PORT));
    if (udp_connection == NULL) {
        printf("Failed to create UDP connection\n");
        return;
    }
    uip_udp_bind(udp_connection, UIP_HTONS(LOCAL_PORT));
}

void send_device_state() {
    if (udp_connection) {
        uip_udp_packet_send(udp_connection, device_state, strlen(device_state));
        printf("Device state sent: %s\n", device_state);
    }
}

int main() {
    printf("UDP client example\n");

    init_udp_connection();

    while (1) {
        send_device_state();
        sleep(5);
    }

    return 0;
}