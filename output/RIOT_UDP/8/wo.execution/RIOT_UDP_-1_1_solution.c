#include <string.h>
#include <stdio.h>
#include <net/af.h>
#include <net/sock/udp.h>
#include <xtimer.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5U * 1000000U)

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET6, .port = SERVER_PORT };
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
    char message[] = "work or not work";
    int res;

    printf("Starting UDP client example\n");

    /* Parse server's address */
    res = sock_udp_str2ep(&server, SERVER_ADDR);
    if (res < 0) {
        printf("Error: unable to parse server address\n");
        return 1;
    }

    /* Open UDP socket */
    res = sock_udp_create(&sock, &local, NULL, 0);
    if (res < 0) {
        printf("Error: unable to open UDP socket\n");
        return 1;
    }

    while (1) {
        /* Send message to server */
        res = sock_udp_send(&sock, message, strlen(message), &server);
        if (res < 0) {
            printf("Error: unable to send message\n");
        } else {
            printf("Message sent to %s\n", SERVER_ADDR);
        }

        /* Wait for the next transmission */
        xtimer_usleep(INTERVAL);
    }

    /* Clean-up is typically unreachable due to the infinite loop */
    sock_udp_close(&sock);

    return 0;
}