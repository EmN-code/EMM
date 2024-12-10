
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_MSG "work"
#define DEVICE_STATE_MSG_LEN 4
#define INTERVAL_MS (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t remote = SOCK_IPV6_EP_ANY;

static void send_device_state(void)
{
    char buf[DEVICE_STATE_MSG_LEN];
    memcpy(buf, DEVICE_STATE_MSG, DEVICE_STATE_MSG_LEN);

    if (sock_udp_send(&sock, buf, DEVICE_STATE_MSG_LEN, &remote) < 0) {
        puts("Error sending UDP message");
    } else {
        puts("Device state sent");
    }
}

int main(void)
{
    // Initialize the UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    // Set the remote endpoint
    remote.family = AF_INET;
    remote.port = UDP_SERVER_PORT;
    if (ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, UDP_SERVER_IP) == NULL) {
        puts("Error parsing server IP address");
        return 1;
    }

    // Send device state every 5 seconds
    while (1) {
        send_device_state();
        xtimer_usleep(INTERVAL_MS);
    }

    // Close the UDP socket
    sock_udp_close(&sock);

    return 0;
}
