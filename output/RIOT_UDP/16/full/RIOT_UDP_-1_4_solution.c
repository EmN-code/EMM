#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5U * US_PER_SEC)

int connect_to_server(sock_udp_ep_t *remote) {
    int rc;
    remote->family = AF_INET;
    remote->port = SERVER_PORT;

    while (1) {
        rc = inet_pton(AF_INET, SERVER_IP, &remote->addr.ipv4);
        if (rc != 1) {
            printf("Error in IP conversion. Retrying in 1 second...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("IP conversion succeeded.\n");
            break;
        }
    }
    return 0;
}

int send_state(sock_udp_t *sock, sock_udp_ep_t *remote) {
    const char *state_msg = "work";
    while (1) {
        ssize_t res = sock_udp_send(sock, state_msg, strlen(state_msg), remote);
        if (res < 0) {
            printf("Failed to send message. Retrying in 1 second...\n");
            xtimer_sleep(1);
            continue;
        }
        printf("Message sent successfully.\n");
        xtimer_usleep(INTERVAL);
    }
    return 0;
}

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    connect_to_server(&remote);

    send_state(&sock, &remote);

    return 0;
}