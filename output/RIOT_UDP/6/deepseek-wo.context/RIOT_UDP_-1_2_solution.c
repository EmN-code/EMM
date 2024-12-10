
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL_MS (5 * 1000U)

static sock_udp_t sock;
static sock_udp_ep_t remote;

void send_state(const char *state) {
    char buf[32];
    snprintf(buf, sizeof(buf), "Device state: %s", state);
    ssize_t res = sock_udp_send(&sock, buf, strlen(buf), &remote);
    if (res < 0) {
        puts("Error sending UDP message");
    } else {
        printf("Sent: %s\n", buf);
    }
}

int main(void) {
    // Initialize the UDP socket
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = 12345; // Local port to bind to

    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    // Set up the remote endpoint
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    if (ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, SERVER_IP) == NULL) {
        puts("Error parsing server IP address");
        return 1;
    }

    // Send device state every 5 seconds
    while (1) {
        send_state("work");
        xtimer_sleep(INTERVAL_MS / MS_PER_SEC);
        send_state("not work");
        xtimer_sleep(INTERVAL_MS / MS_PER_SEC);
    }

    // Close the socket (this will never be reached in this example)
    sock_udp_close(&sock);

    return 0;
}
