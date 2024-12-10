#include <xtimer.h>
#include <thread.h>
#include <net/sock/udp.h>
#include <stdio.h>

#ifndef SERVER_ADDR
#define SERVER_ADDR "47.102.103.1"
#endif

#ifndef SERVER_PORT
#define SERVER_PORT (20001)
#endif

#define RECONNECT_INTERVAL (1U)            /**< Reconnect every second if connection fails */
#define MESSAGE_INTERVAL (5U)              /**< Send device status every 5 seconds */

static sock_udp_ep_t remote;

void initialize_socket(sock_udp_t *sock) {
    int res;
    while (1) {
        res = sock_udp_create(sock, NULL, NULL, 0);
        if (res != 0) {
            printf("Error: Could not create UDP socket (%d)\n", res);
            xtimer_sleep(RECONNECT_INTERVAL);
        } else {
            printf("Socket created successfully\n");
            break;
        }
    }

    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_ADDR, &remote.addr.ipv4);
}

void send_device_state(sock_udp_t *sock, const char *state) {
    ssize_t res = sock_udp_send(sock, state, strlen(state), &remote);
    if (res <= 0) {
        printf("Error sending data (%d)\n", (int)res);
    } else {
        printf("State sent successfully: %s\n", state);
    }
}

int main(void) {
    sock_udp_t sock;
    char *device_state = "work";

    initialize_socket(&sock);

    while (1) {
        send_device_state(&sock, device_state);
        xtimer_sleep(MESSAGE_INTERVAL);
        // Toggle the device state for demonstration
        device_state = (strcmp(device_state, "work") == 0) ? "not work" : "work";
    }

    return 0;
}