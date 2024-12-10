#include <ztimer.h>
#include <sock/udp.h>
#include <net/inet/pton.h>  // for inet_pton
#include <xtimer.h>         // Required for timer

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT (20001)
#define MESSAGE "work"
#define SLEEP_INTERVAL (5U * 1000000U)  // 5 seconds in microseconds

int main(void) {
    // Define UDP endpoint structure
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    // Convert the IP address from string to binary form
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error: unable to parse IP address");
        return 1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, &remote, 0) != 0) {
        puts("Error: Unable to create UDP socket");
        return 1;
    }

    while (1) {
        if (sock_udp_send(&sock, MESSAGE, sizeof(MESSAGE), &remote) < 0) {
            puts("Error: Unable to send UDP packet");
        } else {
            puts("Message sent");
        }
        xtimer_usleep(SLEEP_INTERVAL);
    }
    return 0;
}