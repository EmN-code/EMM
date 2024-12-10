
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL_MS (5 * 1000000U)  // 5 seconds in microseconds

static void send_state(void) {
    static int state = 0;  // 0 for "not work", 1 for "work"
    char message[20];

    if (state == 0) {
        strcpy(message, "not work");
        state = 1;
    } else {
        strcpy(message, "work");
        state = 0;
    }

    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, SERVER_IP);

    sock_udp_t sock;
    sock_udp_create(&sock, NULL, &remote, 0);

    ssize_t res = sock_udp_send(&sock, message, strlen(message), NULL);
    if (res < 0) {
        puts("Failed to send message");
    } else {
        printf("Sent: %s\n", message);
    }

    sock_udp_close(&sock);
}

int main(void) {
    while (1) {
        send_state();
        xtimer_usleep(INTERVAL_MS);
    }

    return 0;
}
