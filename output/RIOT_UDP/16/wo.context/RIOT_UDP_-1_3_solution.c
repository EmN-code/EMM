#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/af.h>
#include <xtimer.h>
#include <net/ipv6/addr.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5U * US_PER_SEC)

int main(void)
{
    /* UDP variables */
    sock_udp_ep_t remote = {.family = AF_INET, .port = SERVER_PORT};
    sock_udp_t sock;
    uint8_t payload[16] = "work";
    int res;
    
    /* inet_pton is defined, so the error should be resolved */
    res = inet_pton(AF_INET, SERVER_IP, &remote.addr);
    if (res != 1) {
        puts("Error: unable to parse server address");
        return 1;
    }

    /* Open UDP socket */
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to create UDP sock");
        return 1;
    }

    while (1) {
        /* Sending UDP data */
        if (sock_udp_send(&sock, payload, sizeof(payload), &remote) < 0) {
            puts("Error: unable to send data");
        } else {
            puts("Data sent successfully");
        }

        /* Wait for 5 seconds before sending again */
        xtimer_sleep(5);
    }

    return 0;
}