#include <xtimer.h>
#include <string.h>
#include <stdio.h>
#include <netinet/in.h>
#include <net/sock/udp.h>

#define SERVER_IP   "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL   5

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    sock_udp_t sock;
    char *device_state = "work";
    uint32_t next_send_time = xtimer_now().ticks32 + SEND_INTERVAL * US_PER_SEC;

    // Convert server IP string to network address
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: unable to convert server IP address\n");
        return 1;
    }

    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        printf("Error: unable to create UDP socket\n");
        return 1;
    }

    while (1) {
        // Send device state
        if (sock_udp_send(&sock, device_state, strlen(device_state), &remote) < 0) {
            printf("Error: unable to send data\n");
        }
        else {
            printf("Sent: %s\n", device_state);
        }

        // Wait for the next send interval
        xtimer_sleep(SEND_INTERVAL);
    }

    sock_udp_close(&sock);
    return 0;
}
