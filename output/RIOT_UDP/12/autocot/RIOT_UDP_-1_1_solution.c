#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/af.h>
#include <net/sock/udp.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE_INTERVAL (5U * US_PER_SEC)  // 5 seconds

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t server = {
        .family = AF_INET,
        .port = SERVER_PORT
    };

    // Resolve server address
    if (ipv6_addr_from_str((ipv6_addr_t *)&server.addr.ipv6, SERVER_ADDR) == NULL) {
        puts("Error: unable to parse server address");
        return 1;
    }

    // Open UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to create socket");
        return 1;
    }

    char message[] = "Device State: work";
    while (1) {
        if (sock_udp_send(&sock, message, sizeof(message), &server) < 0) {
            puts("Error: unable to send message");
        } else {
            puts("Message sent");
        }

        // Wait for 5 seconds
        xtimer_sleep(MESSAGE_INTERVAL / US_PER_SEC);
    }
    
    // Close socket (although in our case this will never be reached)
    sock_udp_close(&sock);
    
    return 0;
}