
#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define BUFF_LEN 512
#define DELAY_MS (5 * 1000 * 1000U)  // 5 seconds in microseconds

static void udp_send_state(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    sock_udp_t sock;
    char buf[BUFF_LEN];
    int res;

    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }

    while (1) {
        // Simulate device state: "work" or "not work"
        const char *state = (rand() % 2 == 0) ? "work" : "not work";
        snprintf(buf, BUFF_LEN, "Device state: %s", state);

        res = sock_udp_send(&sock, buf, strlen(buf), &remote);
        if (res < 0) {
            puts("Error sending UDP message");
        } else {
            printf("Sent: %s\n", buf);
        }

        xtimer_usleep(DELAY_MS);
    }

    sock_udp_close(&sock);
}

int main(void)
{
    puts("UDP client starting");
    udp_send_state();
    return 0;
}
