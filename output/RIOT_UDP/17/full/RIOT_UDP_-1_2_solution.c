#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include "net/ipv6/addr.h"
#include "net/sock/udp.h"

#define SERVER_ADDRESS "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5U * US_PER_SEC) // 5 seconds interval

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    char state_message[16];
    int res;
    
    // Convert the string IP address to a struct in_addr
    if (inet_pton(AF_INET, SERVER_ADDRESS, &remote.addr.ipv6) != 1) {
        puts("Error: unable to parse server address");
        return 1;
    }

    // Create UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return 1;
    }

    // Prepare state message
    strncpy(state_message, "work", sizeof(state_message));

    while (1) {
        res = sock_udp_send(&sock, state_message, strlen(state_message), &remote);
        if (res < 0) {
            printf("Error sending message: %ld\n", (long)res);
        } else {
            puts("State message sent");
        }
        xtimer_sleep(SEND_INTERVAL);
    }

    // Close the socket - in a typical UDP server, this may not be necessary
    sock_udp_close(&sock);

    return 0;
}