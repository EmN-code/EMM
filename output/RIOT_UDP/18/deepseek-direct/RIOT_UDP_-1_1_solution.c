
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
    inet_pton(AF_INET, UDP_SERVER_IP, &remote.addr.ipv4);

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Failed to create UDP socket");
        return;
    }

    if (sock_udp_send(&sock, message, strlen(message), &remote) < 0) {
        puts("Failed to send UDP message");
    } else {
        printf("Sent UDP message: %s\n", message);
    }

    sock_udp_close(&sock);
}

int main(void) {
    puts("RIOT UDP Client for ESP32");

    while (1) {
        // Simulate device state
        const char *device_state = (rand() % 2 == 0) ? DEVICE_STATE_WORK : DEVICE_STATE_NOT_WORK;
        send_udp_message(device_state);

        xtimer_usleep(SEND_INTERVAL);
    }

    return 0;
}
