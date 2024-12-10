
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL_MS (5 * 1000000U)  // 5 seconds in microseconds

static char state[] = "work";  // Initial state

void send_state(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    int res = sock_udp_send(NULL, state, strlen(state), &remote, NULL);
    if (res < 0) {
        puts("Failed to send UDP packet");
    } else {
        printf("Sent state: %s\n", state);
    }
}

int main(void)
{
    puts("UDP Client for ESP32 started");

    while (1) {
        send_state();
        xtimer_usleep(INTERVAL_MS);
    }

    return 0;
}
