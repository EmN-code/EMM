#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <xtimer.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL 5 // seconds

#include <net/ipv4/addr.h>
#include <sock/udp.h>

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET,
                             .port = SERVER_PORT };

    // Convert the IP address from string format to a usable format
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error: unable to parse IP address.");
        return 1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error: unable to create UDP socket.");
        return 1;
    }

    char *work_state = "work";
    char *not_work_state = "not work";
    char *current_state = work_state;

    while (1) {
        if (sock_udp_send(&sock, current_state, strlen(current_state), &remote) < 0) {
            puts("Error: unable to send data.");
        } else {
            puts("Data sent successfully.");
        }

        // Alternate state for demonstration purposes
        current_state = (strcmp(current_state, work_state) == 0) ? not_work_state : work_state;

        // Wait for defined interval before sending the next message
        xtimer_sleep(SEND_INTERVAL);
    }

    // Close the socket (although never reached here in this example)
    sock_udp_close(&sock);

    return 0;
}