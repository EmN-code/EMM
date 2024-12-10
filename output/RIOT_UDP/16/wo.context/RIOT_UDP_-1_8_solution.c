#include <stdio.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <xtimer.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE_INTERVAL (5LU * US_PER_SEC)

void send_device_state(void) {
    sock_udp_ep_t server = { .family = AF_INET, .netif = SOCK_ADDR_ANY_NETIF };
    sock_udp_t sock;

    inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4);
    server.port = SERVER_PORT;

    if (sock_udp_create(&sock, NULL, &server, 0) < 0) {
        puts("Error: Could not create UDP socket");
        return;
    }

    while (1) {
        const char *message = "work";  // Change to "not work" as needed
        if (sock_udp_send(&sock, message, strlen(message), &server) < 0) {
            puts("Error: Could not send message");
        } else {
            puts("Message sent");
        }
        xtimer_sleep(MESSAGE_INTERVAL);
    }

    sock_udp_close(&sock);
}

int main(void) {
    puts("Starting UDP client");
    send_device_state();
    return 0;
}