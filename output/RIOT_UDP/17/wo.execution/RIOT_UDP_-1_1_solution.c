#include <xtimer.h>
#include <net/af.h>
#include <net/sock/udp.h>
#include <stdio.h>
#include <string.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5U * 1000000U) // 5 seconds

typedef struct {
    bool working;
} device_state_t;

void send_device_state(sock_udp_t *sock, device_state_t *state) {
    sock_udp_ep_t server = { .family = AF_INET,
                            .port = SERVER_PORT };
    
    // Parse server address
    if (sock_udp_str2ep(&server, SERVER_ADDR) < 0) {
        printf("Failed to parse server address\n");
        return;
    }

    // Prepare message
    const char *message = state->working ? "work" : "not work";

    // Send message
    if (sock_udp_send(sock, message, strlen(message), &server) <= 0) {
        printf("Failed to send message\n");
    }
    else {
        printf("Message sent: %s\n", message);
    }
}

int main(void) {
    sock_udp_t sock;
    xtimer_ticks32_t last_wakeup = xtimer_now();
    device_state_t state = { .working = true }; // Initial state

    // Initialize the UDP connection
    sock_udp_create(&sock, NULL, NULL, 0);

    while (1) {
        send_device_state(&sock, &state);
        xtimer_periodic_wakeup(&last_wakeup, SEND_INTERVAL);
    }

    return 0;
}