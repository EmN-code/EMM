
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/udp.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_MSG "work"
#define SEND_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;

void send_device_state(void)
{
    char buf[64];
    int res;

    snprintf(buf, sizeof(buf), "%s", DEVICE_STATE_MSG);
    res = sock_udp_send(&sock, buf, strlen(buf), &remote);
    if (res < 0) {
        puts("Error sending UDP message");
    } else {
        printf("Sent: %s\n", buf);
    }
}

int main(void)
{
    int res;

    // Initialize the UDP socket
    res = sock_udp_create(&sock, NULL, &remote, 0);
    if (res < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    // Set the remote endpoint
    remote.family = AF_INET;
    remote.port = UDP_SERVER_PORT;
    inet_pton(AF_INET, UDP_SERVER_IP, &remote.addr.ipv4);

    // Send device state every 5 seconds
    while (1) {
        send_device_state();
        xtimer_usleep(SEND_INTERVAL);
    }

    // Close the socket (this part is unreachable in this example)
    sock_udp_close(&sock);

    return 0;
}
