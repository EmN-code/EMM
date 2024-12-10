
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE_MSG "work"
#define SEND_INTERVAL (5 * 1000 * 1000U) /* 5 seconds in microseconds */

static sock_udp_ep_t server = { .port = SERVER_PORT, .family = AF_INET };
static char buf[64];

void send_device_state(void)
{
    int res;
    sock_udp_t sock;

    /* Initialize the UDP socket */
    if (sock_udp_create(&sock, NULL, &server, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }

    /* Prepare the message */
    strncpy(buf, DEVICE_STATE_MSG, sizeof(buf));

    /* Send the message */
    res = sock_udp_send(&sock, buf, strlen(buf), NULL);
    if (res < 0) {
        puts("Error sending UDP message");
    } else {
        printf("Sent device state: %s\n", buf);
    }

    /* Close the socket */
    sock_udp_close(&sock);
}

int main(void)
{
    /* Initialize the server address */
    if (ipv4_addr_from_str((ipv4_addr_t *)&server.addr.ipv4, SERVER_IP) == NULL) {
        puts("Error parsing server IP address");
        return 1;
    }

    while (1) {
        send_device_state();
        xtimer_usleep(SEND_INTERVAL);
    }

    return 0;
}
