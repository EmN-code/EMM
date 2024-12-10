#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5U * US_PER_SEC)

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_ADDR, &server.addr.ipv4);
    
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to create socket");
        return 1;
    }
    
    char *data = "Device state: work";

    while (1) {
        if (sock_udp_send(&sock, data, strlen(data), &server) < 0) {
            puts("Error: unable to send message");
        } else {
            puts("Message sent successfully");
        }
        xtimer_sleep(SEND_INTERVAL);
    }

    sock_udp_close(&sock);
    return 0;
}