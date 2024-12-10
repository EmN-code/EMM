
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_WORK "work"
#define DEVICE_STATE_NOT_WORK "not work"
#define SEND_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static void send_udp_message(const char *message) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = UDP_SERVER_PORT };
    inet_addr_from_str(remote.addr.ipv4, UDP_SERVER_IP);

    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = 0;  // Let the system assign a local port

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }

    if (sock_udp_send(&sock, message, strlen(message), &remote) < 0) {
        puts("Error sending UDP message");
    } else {
        printf("Sent: %s\n", message);
    }

    sock_udp_close(&sock);
}

int main(void) {
    puts("RIOT UDP Client for ESP32");

    while (1) {
        // Simulate device state
        const char *state = (rand() % 2 == 0) ? DEVICE_STATE_WORK : DEVICE_STATE_NOT_WORK;
        send_udp_message(state);

        xtimer_usleep(SEND_INTERVAL);
    }

    return 0;
}
