#include <xtimer.h>
#include <kernel_defines.h>
#include <net/ipv6/addr.h>
#include <net/ipv4/addr.h>
#include <stdio.h>
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE_INTERVAL (5U*US_PER_SEC)

static sock_udp_ep_t remote;

int main(void) {
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    /* Initialize the remote endpoint structure */
    remote.family = AF_INET;
    remote.port = SERVER_PORT;

    /* Convert server IP address from string to binary form */
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: unable to parse IPv4 address\n");
        return 1;
    }

    char send_buf[] = "work";

    while (1) {
        ssize_t res = sock_udp_send(NULL, send_buf, sizeof(send_buf), &remote);

        if (res <= 0) {
            printf("Error sending message: %d\n", (int)res);
        }
        else {
            printf("Sent message: %s\n", send_buf);
        }

        /* Wait for the next interval */
        xtimer_sleep(MESSAGE_INTERVAL / US_PER_SEC);
    }

    return 0;
}