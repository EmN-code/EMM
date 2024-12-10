#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "sys/socket.h"
#include "net/ipv6/addr.h"
#include "netinet/in.h"
#include "stdio.h"
#include "periph_conf.h"
#include "xtimer.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define UDP_MESSAGE_INTERVAL (5U * US_PER_SEC)  // interval in microseconds

int main(void)
{
    sock_udp_t sock;
    sock_udp_ep_t remote = { .family = AF_INET,
                             .addr.ipv6 = { 0 },  
                             .port = SERVER_PORT };

    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, SERVER_ADDR);
    int res = sock_udp_create(&sock, NULL, NULL, 0);
    if (res < 0) {
        printf("Error initializing UDP socket\n");
        return 1;
    }

    while (1) {
        const char *message = "work";
        ssize_t result = sock_udp_send(&sock, message, strlen(message), &remote);
        if (result < 0) {
            printf("Error sending UDP packet\n");
        }
        else {
            printf("Successfully sent UDP packet\n");
        }
        xtimer_sleep(5);  // sleep for 5 seconds
    }

    sock_udp_close(&sock);

    return 0;
}