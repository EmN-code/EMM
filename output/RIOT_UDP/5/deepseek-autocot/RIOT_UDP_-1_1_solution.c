
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_MSG "work"
#define DEVICE_STATE_MSG_LEN 4
#define INTERVAL_MS (5 * 1000000U)  // 5 seconds in microseconds

static void send_udp_message(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = UDP_SERVER_PORT };
    sock_udp_t sock;
    char buf[DEVICE_STATE_MSG_LEN];

    // Convert IP address from string to binary form
    if (inet_pton(AF_INET, UDP_SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error converting IP address");
        return;
    }

    // Initialize the UDP socket
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }

    // Prepare the message
    strncpy(buf, DEVICE_STATE_MSG, DEVICE_STATE_MSG_LEN);

    // Send the message
    if (sock_udp_send(&sock, buf, DEVICE_STATE_MSG_LEN, NULL) < 0) {
        puts("Error sending UDP message");
    } else {
        printf("Sent: %s\n", buf);
    }

    // Close the socket
    sock_udp_close(&sock);
}

int main(void) {
    puts("RIOT UDP Client for ESP32");

    while (1) {
        send_udp_message();
        xtimer_usleep(INTERVAL_MS);
    }

    return 0;
}
