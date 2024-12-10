#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/ipv4/addr.h>
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL 5000000 // 5 seconds in microseconds

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    
    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);

    const char *state_work = "work";
    const char *state_not_work = "not work";
    bool is_working = true;  // alternates between true and false for the example

    while (1) {
        const char *state = is_working ? state_work : state_not_work;

        ssize_t res = sock_udp_send(&sock, state, strlen(state), &remote);
        if (res < 0) {
            printf("Error sending UDP packet: %d\n", (int)res);
        }

        xtimer_usleep(SEND_INTERVAL);
        is_working = !is_working;  // Toggle the working state
    }

    return 0;
}