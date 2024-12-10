#include <xtimer.h>
#include <string.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>

#define SERVER_PORT 20001
#define SERVER_IP "47.102.103.1"
#define SENDER_PORT 10000

int main(void)
{
    // UDP Socket
    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
    sock_udp_ep_t remote;
    sock_udp_t socket;
    ssize_t res;

    // Configure remote endpoint
    remote.family = AF_INET6;
    remote.port = SERVER_PORT;
    if (inet_pton(AF_INET6, SERVER_IP, &remote.addr.ipv6) != 1) {
        puts("Error: unable to parse IPv6 address");
        return 1;
    }

    // Bind socket
    local.port = SENDER_PORT;
    res = sock_udp_create(&socket, &local, NULL, 0);
    if (res < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    // Sending loop
    char *WORKING_STATE_MSG = "Device is working";
    char *NOT_WORKING_STATE_MSG = "Device is not working";
    bool is_working = true; // Placeholder for actual device state

    while (1) {
        // Toggle is_working variable as an example
        is_working = !is_working;

        // Send message based on device state
        if (is_working) {
            res = sock_udp_send(&socket, WORKING_STATE_MSG, strlen(WORKING_STATE_MSG), &remote);
        } else {
            res = sock_udp_send(&socket, NOT_WORKING_STATE_MSG, strlen(NOT_WORKING_STATE_MSG), &remote);
        }

        if (res < 0) {
            printf("Error sending message\n");
        } else {
            printf("Sent: %s\n", is_working ? WORKING_STATE_MSG : NOT_WORKING_STATE_MSG);
        }

        // Wait 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}