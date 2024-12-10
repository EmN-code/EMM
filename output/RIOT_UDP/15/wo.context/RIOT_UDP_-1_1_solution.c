#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work" // or "not work"
#define SEND_INTERVAL 5 // time in seconds

int main(void) {
    uint8_t buffer[64];
    sock_udp_ep_t server = { .family = AF_INET6, .port = SERVER_PORT };
    sock_udp_t sock;
    int rc;

    printf("Starting UDP client\n");

    // Initialize sending the state
    ipv6_addr_from_str((ipv6_addr_t *)&server.addr, SERVER_IP);

    while (1) {
        rc = sock_udp_create(&sock, NULL, NULL, 0);
        if (rc < 0) {
            printf("Error creating UDP socket: %d\n", rc);
            xtimer_sleep(1);
            continue;
        }

        strncpy((char *)buffer, DEVICE_STATE, sizeof(buffer));
        rc = sock_udp_send(&sock, buffer, strlen((char *)buffer), &server);
        if (rc < 0) {
            printf("Error sending UDP packet: %d\n", rc);
            xtimer_sleep(1);
            sock_udp_close(&sock);
            continue;
        } else {
            printf("Device state sent: %s\n", DEVICE_STATE);
            sock_udp_close(&sock);
        }

        xtimer_sleep(SEND_INTERVAL);
    }
    return 0;
}