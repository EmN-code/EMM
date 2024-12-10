#include "xtimer.h"
#include "net/ipv4/addr.h"
#include "net/af.h"
#include "net/sock/udp.h"
#include <string.h>
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5U * 1000U * 1000U) // 5 seconds in microseconds

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    uint8_t remote_addr[16];
    char *message = "work";  // or "not work" depending on the state

    // Convert the IP address from string format to network bytes
    if (inet_pton(AF_INET, SERVER_IP, remote_addr) != 1) {
        printf("Error: Could not parse IP address %s\n", SERVER_IP);
        return -1;
    }
    memcpy(&remote.addr.ipv4, remote_addr, sizeof(remote.addr.ipv4));

    // Socket initialization
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: Unable to create socket\n");
        return -1;
    }

    while (1) {
        ssize_t res = sock_udp_send(&sock, message, strlen(message), &remote);
        if (res < 0) {
            printf("Error sending packet\n");
        }
        else {
            printf("Successfully sent message: %s\n", message);
        }
        xtimer_usleep(SEND_INTERVAL);
    }
    
    // Socket will not be closed as the program runs indefinitely.

    return 0;
}