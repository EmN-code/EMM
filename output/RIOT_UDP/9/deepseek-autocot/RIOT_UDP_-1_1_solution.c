
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL_MS (5 * 1000000U)  // 5 seconds in microseconds

static void send_state(void) {
    static const char *state = "work";  // or "not work" based on device state

    sock_udp_ep_t remote = {
        .family = AF_INET,
        .port = SERVER_PORT,
    };

    if (sock_udp_str2ep(&remote, SERVER_IP) < 0) {
        puts("Error: unable to parse server address");
        return;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return;
    }

    if (sock_udp_send(&sock, state, strlen(state), NULL) < 0) {
        puts("Error: unable to send UDP packet");
    } else {
        printf("Sent state: %s\n", state);
    }

    sock_udp_close(&sock);
}

int main(void) {
    puts("RIOT UDP Client for ESP32");

    while (1) {
        send_state();
        xtimer_usleep(INTERVAL_MS);
    }

    return 0;
}
