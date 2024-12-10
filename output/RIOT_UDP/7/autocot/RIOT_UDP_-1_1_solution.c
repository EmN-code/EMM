#include <xtimer.h>
#include <string.h>
#include <stdio.h>
#include <net/af.h>
#include <net/sock/udp.h>

#define SERVER_PORT 20001
#define SERVER_ADDR "47.102.103.1"
#define SEND_INTERVAL 5U

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    if (netif_set_ipaddr(&server.netif, SERVER_ADDR) < 0) {
        printf("Error setting server address\n");
        return 1;
    }

    xtimer_sleep(5);  // wait for network to settle

    char message[32];
    while (1) {
        strcpy(message, "work");  // Replace "work" with actual device state logic
        sock_udp_send(NULL, message, strlen(message), &server);
        xtimer_sleep(SEND_INTERVAL);
    }

    return 0;
}
