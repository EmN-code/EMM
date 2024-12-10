#include "xtimer.h"
#include "sock_udp.h"
#include "net/ipv4/addr.h"
#include <stdio.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define BUFFER_SIZE 64
#define SLEEP_INTERVAL (5 * US_PER_SEC)  // 5 seconds in microseconds

int main(void) {
    printf("[INFO] Starting UDP client example...\n");

    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_ADDR, &server.addr.ipv4);

    sock_udp_t sock;
    uint8_t buffer[BUFFER_SIZE];
    char *message = "Device working";

    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("[ERROR] Unable to create socket\n");
        return 1;
    }

    while (1) {
        ssize_t res = sock_udp_send(&sock, message, strlen(message), &server);
        if (res < 0) {
            printf("[ERROR] Error sending data\n");
        } else {
            printf("[INFO] Sent %zd bytes\n", res);
        }

        xtimer_sleep(5);
    }

    sock_udp_close(&sock);
    return 0;
}