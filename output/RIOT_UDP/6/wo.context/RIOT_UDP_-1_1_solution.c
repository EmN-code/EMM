#include <string.h>
#include <stdio.h>
#include "xtimer.h"
#include "net/af.h"
#include "net/ipv6/addr.h"
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5U * 1000000U) // 5 seconds

static void send_device_status(void) {
    char *message = "work"; // or "not work" based on device state
    sock_udp_ep_t server = {.family = AF_INET6};
    ipv6_addr_from_str((ipv6_addr_t *)&server.addr.ipv6, SERVER_IP);
    server.port = SERVER_PORT;
    
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }

    if (sock_udp_send(&sock, message, strlen(message), &server) < 0) {
        puts("Error sending UDP message");
    } else {
        printf("Message '%s' sent to %s:%u\n", message, SERVER_IP, SERVER_PORT);
    }

    sock_udp_close(&sock);
}

int main(void) {
    while (1) {
        send_device_status();
        xtimer_sleep(SEND_INTERVAL / 1000000U);
    }
    return 0;
}