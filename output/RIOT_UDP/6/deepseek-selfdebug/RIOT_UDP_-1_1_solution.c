
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

static sock_udp_ep_t server = { .port = UDP_SERVER_PORT, .family = AF_INET };
static sock_udp_t sock;

static void send_device_state(void)
{
    int res;
    sock_udp_ep_t remote = server;
    remote.addr.ipv4.u32 = ipv4_addr_from_str(UDP_SERVER_IP);

    res = sock_udp_send(&sock, DEVICE_STATE_MSG, DEVICE_STATE_MSG_LEN, &remote);
    if (res < 0) {
        puts("Error sending UDP message");
    } else {
        printf("Sent device state: %s\n", DEVICE_STATE_MSG);
    }
}

int main(void)
{
    int res;

    // Initialize the UDP socket
    res = sock_udp_create(&sock, NULL, &server, 0);
    if (res < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    while (1) {
        send_device_state();
        xtimer_usleep(INTERVAL_MS);
    }

    // Close the UDP socket
    sock_udp_close(&sock);

    return 0;
}
