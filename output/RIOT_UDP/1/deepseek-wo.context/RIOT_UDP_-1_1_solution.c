
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE_MSG "work"
#define SEND_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };

static void send_device_state(void)
{
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    char buf[] = DEVICE_STATE_MSG;

    // Initialize the socket
    if (sock_udp_create(&sock, &local, &server, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }

    // Send the device state message
    if (sock_udp_send(&sock, buf, strlen(buf), NULL) < 0) {
        puts("Error sending UDP message");
    } else {
        printf("Sent: %s\n", buf);
    }

    // Close the socket
    sock_udp_close(&sock);
}

int main(void)
{
    // Convert server IP address from string to binary form
    if (ipv4_addr_from_str((ipv4_addr_t *)&server.addr.ipv4, SERVER_IP) == NULL) {
        puts("Error parsing server IP address");
        return 1;
    }

    while (1) {
        send_device_state();
        xtimer_usleep(SEND_INTERVAL);
    }

    return 0;
}