#include "netinet/in.h"
#include "sys/socket.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "net/gcoap.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE "work"
#define SLEEP_DURATION (5 * 1000000)  /* 5 seconds in microseconds */

int main(void) {
    sock_udp_ep_t server;
    sock_udp_t sock;

    /* Create UDP socket */
    if(sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error creating UDP socket\n");
        return -1;
    }

    /* Set the server address */
    server.family = AF_INET;
    server.port = SERVER_PORT;
    if (inet_pton(AF_INET, SERVER_ADDR, &server.addr) != 1) {
        printf("Error setting server address\n");
        return -1;
    }

    while (1) {
        if (sock_udp_send(&sock, MESSAGE, sizeof(MESSAGE), &server) < 0) {
            printf("Error sending message\n");
        } else {
            printf("Message sent: %s\n", MESSAGE);
        }

        sock_udp_close(&sock);

        /* Sleep for the specified duration */
        xtimer_usleep(SLEEP_DURATION);  /* Ensure this function is available from RIOT for delays */
    }

    return 0;
}