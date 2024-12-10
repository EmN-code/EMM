#include "udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "sys/socket.h"
#include "net/sock/udp.h"
#include "xtimer.h"
#include "net/af.h"

#define UDP_SERVER_IPV4 "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define SEND_INTERVAL (5U * US_PER_SEC)

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET6, .port = UDP_SERVER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&server.addr, UDP_SERVER_IPV4);

    sock_udp_t sock;
    int rc;

    while (1) {
        rc = sock_udp_create(&sock, NULL, &server, 0);
        if (rc < 0) {
            printf("Failed to create UDP sock: %d\n", rc);
            xtimer_sleep(1);
            continue;
        }

        const char *message = "Device is working";
        ssize_t res = sock_udp_send(&sock, message, strlen(message), &server);
        if (res < 0) {
            printf("Failed to send message: %ld\n", res);
            sock_udp_close(&sock);
            xtimer_sleep(1);
            continue;
        }

        printf("Message sent successfully\n");
        xtimer_sleep(SEND_INTERVAL);
    }

    return 0;
}
