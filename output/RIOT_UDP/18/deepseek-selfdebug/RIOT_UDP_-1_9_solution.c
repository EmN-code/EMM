
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"  // or "not work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static void send_state(void) {
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    uint8_t buf[128];
    size_t len;

    // Convert IP address string to binary form
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    remote.port = SERVER_PORT;

    // Prepare the message
    len = snprintf((char *)buf, sizeof(buf), "Device state: %s", DEVICE_STATE);

    // Send the message
    if (sock_udp_send(NULL, buf, len, &remote) < 0) {
        puts("Error sending UDP message");
    } else {
        puts("UDP message sent successfully");
    }
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        send_state();
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
    }

    return 0;
}
