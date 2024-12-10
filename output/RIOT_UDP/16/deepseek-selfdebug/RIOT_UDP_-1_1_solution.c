
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_MSG "work"
#define DEVICE_STATE_MSG_LEN (sizeof(DEVICE_STATE_MSG) - 1)
#define SEND_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static void send_device_state(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = UDP_SERVER_PORT };
    sock_udp_t sock;
    char buf[DEVICE_STATE_MSG_LEN];

    // Initialize the buffer with the device state message
    memcpy(buf, DEVICE_STATE_MSG, DEVICE_STATE_MSG_LEN);

    // Resolve the server IP address
    if (sock_udp_resolve_host(&remote, UDP_SERVER_IP) < 0) {
        puts("Error: unable to resolve server IP address");
        return;
    }

    // Initialize the UDP socket
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return;
    }

    // Send the device state message
    if (sock_udp_send(&sock, buf, DEVICE_STATE_MSG_LEN, &remote) < 0) {
        puts("Error: unable to send UDP message");
    } else {
        puts("Device state sent successfully");
    }

    // Close the UDP socket
    sock_udp_close(&sock);
}

int main(void)
{
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        send_device_state();
        xtimer_periodic_wakeup(&last_wakeup, SEND_INTERVAL);
    }

    return 0;
}
