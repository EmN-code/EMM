
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"  // or "not work"
#define INTERVAL_MS (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;

void send_device_state(void)
{
    char buf[32];
    sprintf(buf, "Device state: %s", DEVICE_STATE);

    if (sock_udp_send(&sock, buf, strlen(buf), &remote) < 0) {
        puts("Error sending UDP message");
    } else {
        puts("Device state sent");
    }
}

int main(void)
{
    // Initialize the UDP socket
    remote.port = SERVER_PORT;
    remote.family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error creating UDP socket");
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
