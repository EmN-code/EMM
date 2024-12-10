#include <string.h>  // include necessary header for string operations
#include <stdio.h>    // include necessary header for standard input and output
#include <net/sock/udp.h>  // include header for UDP socket operations
#include <net/inet.h>  // include for inet_pton function
#include <xtimer.h>  // include for timer related functions

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE "work or not work"
#define SLEEP_INTERVAL (5U * 1000000U)  // sleep interval in microseconds

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    sock_udp_t sock;
    uint8_t buf[128];  // buffer to hold incoming messages

    if (inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4) != 1) {
        puts("Error: unable to parse IPv4 address");
        return 1;
    }

    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return 1;
    }

    while (1) {
        if (sock_udp_send(&sock, MESSAGE, strlen(MESSAGE), &server) < 0) {
            puts("Error: unable to send message");
        } else {
            puts("Message sent successfully");
        }

        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}