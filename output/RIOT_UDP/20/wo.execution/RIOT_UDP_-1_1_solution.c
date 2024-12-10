#include <xtimer.h>
#include <stdio.h>
#include <net/af.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001

define SLEEP_INTERVAL (5U * US_PER_SEC) // send every 5 seconds

typedef struct {
    bool is_working;
} device_state_t;

static device_state_t device_state = {true}; // initial state is "work"

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET6, .port = SERVER_PORT };
    if (inet_pton(AF_INET, SERVER_IP, &server.addr) <= 0) {
        printf("Error: cannot set server address\n");
        return 1;
    }

    while (1) {
        sock_udp_t sock;
        if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
            printf("Error: could not create UDP socket\n");
            return 1;
        }

        const char *message = device_state.is_working ? "work" : "not work";

        if (sock_udp_send(&sock, message, strlen(message), &server) < 0) {
            printf("Error: could not send message\n");
        } else {
            printf("Sent message: %s\n", message);
        }

        // Free the socket
        sock_udp_close(&sock);

        // Wait for the defined interval
        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}
