
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_MSG "work"
#define DEVICE_STATE_MSG_LEN (sizeof(DEVICE_STATE_MSG) - 1)
#define INTERVAL_MS (5 * 1000000U)  // 5 seconds in microseconds

static void send_udp_message(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = UDP_SERVER_PORT };
    inet_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, UDP_SERVER_IP);

    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = 0;  // Let the system assign a local port

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }

    if (sock_udp_send(&sock, DEVICE_STATE_MSG, DEVICE_STATE_MSG_LEN, &remote) < 0) {
        puts("Error sending UDP message");
    } else {
        puts("UDP message sent successfully");
    }

    sock_udp_close(&sock);
}

int main(void) {
    puts("Starting UDP client...");

    while (1) {
        send_udp_message();
        xtimer_usleep(INTERVAL_MS);
    }

    return 0;
}
