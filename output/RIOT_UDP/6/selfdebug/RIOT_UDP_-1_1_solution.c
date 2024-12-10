#include <stdio.h>
#include <string.h>
#include <net/ipv6/addr.h>
#include <net/sock/udp.h>
#include <timex.h>
#include <xtimer.h>

#define SERVER_IP   "47.102.103.1"
#define SERVER_PORT 20001
#define SLEEP_TIME  (5U * US_PER_SEC)

int main(void) {
    sock_udp_ep_t server = { .port = SERVER_PORT };
    sock_udp_t sock;
    msg_t msg_queue[8];

    /* configure the IPv6 address for the server */
    ipv6_addr_t addr;
    if (ipv6_addr_from_str(&addr, SERVER_IP) == NULL) {
        printf("Error: unable to parse server address\n");
        return 1;
    }
    memcpy(&server.addr.ipv6, &addr, sizeof(addr));

    /* create UDP socket */
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: unable to create UDP socket\n");
        return 1;
    }

    char *device_state = "work"; /* This would be dynamically determined in real usage */

    /* Main sending loop */
    while (1) {
        if (sock_udp_send(&sock, device_state, strlen(device_state), &server) < 0) {
            printf("Error: failed to send UDP packet\n");
        } else {
            printf("Device state '%s' sent to %s:%u\n", device_state, SERVER_IP, SERVER_PORT);
        }
        xtimer_sleep(SLEEP_TIME / US_PER_SEC);  /* Sleep for 5 seconds */
    }

    /* Close the socket - unreachable code in this example */
    sock_udp_close(&sock);

    return 0;
}