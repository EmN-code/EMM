
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;

static void send_state(void)
{
    char buf[32];
    sprintf(buf, "%s", DEVICE_STATE);

    if (sock_udp_send(&sock, buf, strlen(buf), &remote) < 0) {
        puts("Error sending UDP message");
    } else {
        puts("State sent");
    }
}

int main(void)
{
    // Initialize the UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    // Convert the server IP address from string to binary form
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error converting IP address");
        return 1;
    }
    remote.port = SERVER_PORT;

    // Send device state every 5 seconds
    while (1) {
        send_state();
        xtimer_usleep(INTERVAL);
    }

    // Close the UDP socket
    sock_udp_close(&sock);

    return 0;
}
