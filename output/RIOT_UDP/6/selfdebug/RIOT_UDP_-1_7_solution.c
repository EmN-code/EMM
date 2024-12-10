#include <xtimer.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/af.h>
#include <net/ipv4/addr.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5U * US_PER_SEC)

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    char message_work[] = "work";
    char message_not_work[] = "not work";
    char buffer[64];
    int res;

    /* Convert server IP address */
    if (inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4) != 1) {
        puts("Error: unable to parse IPv4 address");
        return 1;
    }

    while (1) {
        /* Select message to send based on device state */
        const char *message_to_send = (/* condition to check device state */) ? message_work : message_not_work;

        /* Send message to server */
        res = sock_udp_send(NULL, message_to_send, strlen(message_to_send), &server);
        if (res < 0) {
            printf("Error sending message: %d\n", res);
        }
        else {
            printf("Sent message: %s\n", message_to_send);
        }

        /* Wait for the next send interval */
        xtimer_sleep(SEND_INTERVAL);
    }

    return 0;
}
