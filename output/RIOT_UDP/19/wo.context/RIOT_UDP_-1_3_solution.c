#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <xtimer.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define UDP_BUFFER_SIZE 128
#define SEND_INTERVAL (5U * 1000000U) // 5 seconds

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET6, .port = SERVER_PORT };
    int res;

    // Convert the IP address from string format to binary format
    res = inet_pton(AF_INET6, SERVER_IP, &server.addr.ipv6);
    if (res != 1) {
        printf("Error: Unable to convert IP address\n");
        return 1;
    }

    sock_udp_t sock;
    res = sock_udp_create(&sock, NULL, NULL, 0);
    if (res < 0) {
        printf("Error: Unable to create UDP socket\n");
        return 1;
    }

    char message[] = "work";
    while (1) {
        res = sock_udp_send(&sock, message, strlen(message), &server);
        if (res < 0) {
            printf("Error: Unable to send data\n");
        } else {
            printf("Message sent: %s\n", message);
        }
        xtimer_usleep(SEND_INTERVAL);
    }

    return 0;
}