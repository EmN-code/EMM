
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_MSG "work"
#define DEVICE_STATE_MSG_LEN (sizeof(DEVICE_STATE_MSG) - 1)
#define SEND_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_ep_t server = { .family = AF_INET, .port = UDP_SERVER_PORT };
static char buf[DEVICE_STATE_MSG_LEN];

static void send_device_state(void)
{
    sock_udp_t sock;
    sock_udp_ep_t remote = server;

    // Resolve the server IP address
    if (sock_udp_name2ep(&remote, UDP_SERVER_IP) < 0) {
        puts("Error: unable to resolve server address");
        return;
    }

    // Create a UDP socket
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return;
    }

    // Send the device state message
    strncpy(buf, DEVICE_STATE_MSG, DEVICE_STATE_MSG_LEN);
    if (sock_udp_send(&sock, buf, DEVICE_STATE_MSG_LEN, &remote) < 0) {
        puts("Error: unable to send UDP message");
    } else {
        printf("Sent: %s\n", buf);
    }

    // Close the UDP socket
    sock_udp_close(&sock);
}

int main(void)
{
    while (1) {
        send_device_state();
        xtimer_usleep(SEND_INTERVAL);
    }

    return 0;
}
