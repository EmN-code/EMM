#include <string.h>
#include <netinet/in.h>
#include "net/sock/udp.h"
#include "xtimer.h"

#define SERVER_PORT (20001)
#define SERVER_ADDR "47.102.103.1"
#define MESSAGE_INTERVAL (5U)
#define MESSAGE "work"

static sock_udp_ep_t remote = {
    .port = SERVER_PORT
};

static uint8_t buf[128];

int main(void) {
    if (inet_pton(AF_INET, SERVER_ADDR, &remote.addr.ipv4) != 1) {
        printf("Error: unable to parse IPv4 address\n");
        return -1;
    }

    while (1) {
        ssize_t res = sock_udp_send(NULL, MESSAGE, sizeof(MESSAGE), &remote);
        if (res < 0) {
            printf("Error sending packet\n");
        } else {
            printf("Packet sent\n");
        }
        xtimer_sleep(MESSAGE_INTERVAL);
    }

    return 0;
}