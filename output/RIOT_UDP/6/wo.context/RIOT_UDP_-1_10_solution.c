#include <xtimer.h>
#include <string.h>
#include <net/udp.h>
#include <net/af.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define SEND_INTERVAL (5U * US_PER_SEC)

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = UDP_SERVER_PORT };

    if (inet_pton(AF_INET, UDP_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: unable to convert IP address\n");
        return 1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: unable to create socket\n");
        return 1;
    }

    char *state_message = "work";
    while (1) {
        if (sock_udp_send(&sock, state_message, strlen(state_message), &remote) < 0) {
            printf("Error: unable to send message\n");
        }
        else {
            printf("Message sent: %s\n", state_message);
        }
        xtimer_sleep(SEND_INTERVAL / US_PER_SEC);
    }

    return 0;
}