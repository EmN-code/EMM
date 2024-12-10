#include <xtimer.h>
#include <string.h>
#include <stdio.h>
#include <net/sock/tcp.h>
#include <net/ipv6/addr.h>
#include <net/ipv4/addr.h>
#include <net/protnum.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define MSG_INTERVAL (5U * US_PER_SEC)

int main(void) {
    /* Define message and address components */
    const char *message = "work or not work";
    sock_tcp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    int res = inet_pton(AF_INET, SERVER_ADDR, &remote.addr.ipv4);
    if (res <= 0) {
        printf("Error: could not convert server address\n");
        return 1;
    }

    /* Create and connect TCP socket */
    sock_tcp_t sock;
    sock_tcp_create(&sock, NULL, NULL, 0);
    res = sock_tcp_connect(&sock, &remote, 0, 0);
    if (res < 0) {
        printf("Error: could not connect to server\n");
        return 1;
    }

    /* Main loop for sending messages every 5 seconds */
    while (1) {
        res = sock_tcp_write(&sock, message, strlen(message));
        if (res < 0) {
            printf("Error: could not send message\n");
        }
        xtimer_sleep(MSG_INTERVAL);
    }

    /* Close socket (unreachable code in this case) */
    sock_tcp_disconnect(&sock);
    sock_tcp_close(&sock);

    return 0;
}