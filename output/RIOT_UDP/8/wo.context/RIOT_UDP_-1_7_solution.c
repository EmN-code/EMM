#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>  // For inet_pton function
#include <net/sock/udp.h>  // For UDP socket operations
#include <xtimer.h>  // For timer operations

#define SERVER_PORT (20001)
#define SERVER_ADDR "47.102.103.1"
#define MESSAGE_INTERVAL (5U * US_PER_SEC)  // 5 seconds interval

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    if (inet_pton(AF_INET, SERVER_ADDR, &server.addr.ipv4) != 1) {
        printf("Error: Couldn't parse IP address\n");
        return -1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: Unable to create UDP socket\n");
        return -1;
    }

    char *message_on = "Device working";
    char *message_off = "Device not working";

    while (1) {
        // Send the "Device working" message
        if (sock_udp_send(&sock, message_on, strlen(message_on), &server) < 0) {
            printf("Error: Sending message\n");
        }
        else {
            printf("Sent: %s\n", message_on);
        }

        // Wait for the interval
        xtimer_sleep(MESSAGE_INTERVAL);

        // Send the "Device not working" message
        if (sock_udp_send(&sock, message_off, strlen(message_off), &server) < 0) {
            printf("Error: Sending message\n");
        }
        else {
            printf("Sent: %s\n", message_off);
        }

        // Wait for the next interval
        xtimer_sleep(MESSAGE_INTERVAL);
    }

    return 0;
}