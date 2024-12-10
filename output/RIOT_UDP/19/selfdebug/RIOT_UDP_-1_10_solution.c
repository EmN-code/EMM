#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/inet_pton.h>
#include <xtimer.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5U * US_PER_SEC)

int main(void) {
    puts("ESP32 UDP client example");

    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4);

    sock_udp_t sock;
    uint8_t buffer[64];

    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return -1;
    }

    while (1) {
        strcpy((char *)buffer, "work");
        if (sock_udp_send(&sock, buffer, sizeof("work"), &server) < 0) {
            puts("Error sending UDP packet");
        } else {
            puts("Packet sent");
        }
        xtimer_sleep(SEND_INTERVAL);
    }

    return 0;
}