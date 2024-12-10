#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/ipv6/addr.h>
#include <net/sock/udp.h>

#define UDP_SERVER_PORT 20001
#define UDP_SERVER_ADDR "47.102.103.1"
#define SEND_INTERVAL 5

int main(void) {
    /* Define UDP endpoint structure */
    sock_udp_ep_t server = { .family = AF_INET, .port = UDP_SERVER_PORT };

    /* Create UDP socket */
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return -1;
    }

    /* Convert server IP address from text to binary format */
    if (inet_pton(AF_INET, UDP_SERVER_ADDR, &server.addr.ipv4) != 1) {
        puts("Error: Invalid IPv4 address");
        return -1;
    }

    while (1) {
        const char *state_msg = "Device state: work";
        /* Send state message to server */
        if (sock_udp_send(&sock, state_msg, strlen(state_msg), &server) < 0) {
            puts("Error sending message");
            return -1;
        }
        /* Wait for the next interval */
        xtimer_sleep(SEND_INTERVAL);
    }

    return 0;
}