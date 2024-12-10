#include <xtimer.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <net/protnames.h>
#include <net/af.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SLEEP_INTERVAL 5

static const char *MESSAGE_WORK = "work";
static const char *MESSAGE_NOT_WORK = "not work";

int main(void) {
    ipv6_addr_t server_addr;
    uint16_t msg_counter = 0;

    if (ipv6_addr_from_str(&server_addr, SERVER_IP) == NULL) {
        puts("Error: unable to parse server address.");
        return 1;
    }

    sock_udp_ep_t remote = {
        .family = AF_INET6,
        .port = SERVER_PORT
    };
    memcpy(&remote.addr.ipv6, &server_addr, sizeof(server_addr));

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to create UDP socket.");
        return 1;
    }

    while (1) {
        const char *msg = (msg_counter % 2 == 0) ? MESSAGE_WORK : MESSAGE_NOT_WORK;
        if (sock_udp_send(&sock, msg, strlen(msg), &remote) < 0) {
            puts("Error: unable to send message.");
        } else {
            printf("Sent message: %s\n", msg);
        }
        msg_counter++;
        xtimer_sleep(SLEEP_INTERVAL);
    }
    return 0;
}