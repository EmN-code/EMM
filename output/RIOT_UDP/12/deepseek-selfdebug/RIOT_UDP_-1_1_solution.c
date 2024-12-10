
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL_MS (5 * 1000000U)  // 5 seconds in microseconds

static char state_msg[20];

void send_state(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_addr_from_str(remote.addr.ipv4, SERVER_IP);

    int res = sock_udp_send(NULL, state_msg, strlen(state_msg), &remote, NULL);
    if (res < 0) {
        puts("Failed to send UDP message");
    } else {
        printf("Sent: %s\n", state_msg);
    }
}

int main(void) {
    // Initialize the network interface
    if (netif_set_default(netif_iter(NULL)) == NULL) {
        puts("No default network interface found");
        return 1;
    }

    // Set the initial state message
    strcpy(state_msg, "work");

    // Create a timer for periodic sending
    xtimer_t timer;
    xtimer_init(&timer);

    while (1) {
        send_state();
        xtimer_usleep(INTERVAL_MS);
    }

    return 0;
}
