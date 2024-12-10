
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
    if (sock_udp_str2ep(&remote, UDP_SERVER_IP) < 0) {
        puts("Error: unable to parse server address");
        return;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return;
    }

    if (sock_udp_send(&sock, message, strlen(message), &remote) < 0) {
        puts("Error: unable to send UDP message");
    } else {
        printf("Sent: %s\n", message);
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
