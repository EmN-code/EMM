#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <stdio.h>
#include <xtimer.h>

#define SERVER_PORT (20001)
#define CLIENT_MESSAGE "work"
#define INTERVAL (5U * US_PER_SEC)

static const char *server_address = "47.102.103.1";

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, server_address, &server.addr.ipv4);
    
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    
    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error: Unable to create UDP sock\n");
        return -1;
    }
    
    while (1) {
        ssize_t res = sock_udp_send(&sock, CLIENT_MESSAGE, strlen(CLIENT_MESSAGE), &server);
        if (res < 0) {
            printf("Error sending message: %d\n", (int)res);
        } else {
            puts("Message sent\n");
        }
        
        xtimer_sleep(5);
    }
    return 0;
}
