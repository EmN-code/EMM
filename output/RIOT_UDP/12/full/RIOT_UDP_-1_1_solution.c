#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/af.h>
#include <net/ipv6/addr.h>
#include <net/sock/udp.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT (20001)
#define LOCAL_PORT (1337)
#define SEND_INTERVAL (5U * US_PER_SEC)

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
    sock_udp_ep_t remote;
    char state_work[] = "work";
    char state_not_work[] = "not work";
    char *device_state = state_work; // Assuming device is initially working
    int res;

    /* setup local address and port for socket */
    local.port = LOCAL_PORT;

    /* setup remote address and port */
    ipv6_addr_from_str((ipv6_addr_t *) &remote.addr.ipv6, SERVER_ADDR);
    remote.port = SERVER_PORT;
    remote.family = AF_INET6;

    /* create UDP socket */
    while (1) {
        res = sock_udp_create(&sock, &local, NULL, 0);
        if (res != 0) {
            printf("Error creating UDP socket: %d\n", res);
            xtimer_sleep(1);
            continue;
        } else {
            printf("Socket created successfully\n");
            break;
        }
    }

    while (1) {
        /* Send the device state to the server */
        res = sock_udp_send(&sock, device_state, strlen(device_state), &remote);
        if (res < 0) {
            printf("Error sending message: %d\n", res);
        } else {
            printf("Sent device state: %s\n", device_state);
        }

        /* Wait for the next sending interval */
        xtimer_sleep(SEND_INTERVAL / US_PER_SEC);
    }

    /* Close the socket */
    sock_udp_close(&sock);

    return 0;
}