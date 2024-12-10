#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/sock.h>

#define SERVER_PORT 20001
#define MESSAGE_INTERVAL_S 5
#define SERVER_IP "47.102.103.1"

static sock_udp_ep_t remote_endpoint;

int main(void) {
    /* Parsing server address */
    remote_endpoint.family = AF_INET;
    remote_endpoint.port = SERVER_PORT;

    if (inet_pton(AF_INET, SERVER_IP, &remote_endpoint.addr.ipv4) != 1) {
        puts("Error: unable to parse server IP");
        return 1;
    }

    /* Create UDP socket */
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) != 0) {
        puts("Error: unable to create UDP socket");
        return 1;
    }

    /* Message loop */
    while (1) {
        /* Send device state to server */
        char state_message[] = "device state: work";
        if (sock_udp_send(&sock, state_message, sizeof(state_message), &remote_endpoint) < 0) {
            puts("Error: unable to send message");
        }
        /* Wait for the specified interval before sending next message */
        xtimer_sleep(MESSAGE_INTERVAL_S);
    }

    return 0;
}