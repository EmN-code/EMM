#include "sys/socket.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/af.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define RECONNECT_INTERVAL 1
#define SEND_INTERVAL 5
#define STATE_WORK "work"
#define STATE_NOT_WORK "not work"

int send_state(sock_udp_t *sock, sock_udp_ep_t *remote, const char *state) {
    ssize_t result = sock_udp_send(sock, state, strlen(state), remote);
    if (result < 0) {
        printf("Error sending data: %d\n", result);
        return -1;
    }
    printf("State '%s' sent.\n", state);
    return 0;
}

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t remote = {.family = AF_INET, .port = SERVER_PORT};
    ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, SERVER_IP);

    int rc;

    while (1) {
        rc = sock_udp_create(&sock, NULL, &remote, 0);
        if (rc == -EAFNOSUPPORT) {
            printf("Failure to create UDP socket. Retrying...\n");
            ztimer_sleep(RECONNECT_INTERVAL);
            continue;
        } else if (rc != 0) {
            printf("Socket creation failed: %d\n", rc);
            return 1;
        }
        printf("Socket created successfully.\n");
        break;
    }

    while (1) {
        send_state(&sock, &remote, STATE_WORK);
        ztimer_sleep(SEND_INTERVAL);
    }

    sock_udp_close(&sock);
    return 0;
}