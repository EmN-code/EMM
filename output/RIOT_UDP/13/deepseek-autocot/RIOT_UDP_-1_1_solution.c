
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/udp.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_MSG "work"
#define DEVICE_STATE_MSG_LEN (sizeof(DEVICE_STATE_MSG) - 1)
#define SEND_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;

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
    remote.port = UDP_SERVER_PORT;
    remote.family = AF_INET;
    inet_pton(AF_INET, UDP_SERVER_IP, &remote.addr.ipv4);

    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    while (1) {
        send_device_state();
        xtimer_usleep(SEND_INTERVAL);
    }

    // Close the UDP socket
    sock_udp_close(&sock);

    return 0;
}
