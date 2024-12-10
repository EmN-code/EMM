#include <xtimer.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <arpa/inet.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT (20001)
#define SEND_INTERVAL (5U * 1000000U) // 5 seconds

int main(void) {
    puts("ESP32 UDP client program\n");

    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: Unable to create UDP socket\n");
        return 1;
    }

    char *message = "Device status: Work\n";
    while (1) {
        ssize_t res = sock_udp_send(&sock, message, strlen(message), &remote);
        if (res < 0) {
            puts("Error: Unable to send message\n");
        } else {
            printf("Sent %d bytes\n", (int)res);
        }
        xtimer_usleep(SEND_INTERVAL);
    }

    return 0;
}