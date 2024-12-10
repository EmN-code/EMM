#include <udp.h>
#include <sock.h>
#include <net/sock/udp.h>
#include <kernel.h>
#include <stdio.h>
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MSG_INTERVAL (5U)

int main(void) {
    static sock_udp_t sock;
    static sock_udp_ep_t remote;
    static char buffer[128];
    ssize_t res;

    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP socket\n");
        return 1;
    }

    puts("UDP client task started\n");
    while (1) {
        const char *msg = "Device is working";
        strcpy(buffer, msg);

        res = sock_udp_send(&sock, buffer, strlen(buffer), &remote);
        if (res < 0) {
            puts("Error sending message\n");
        } else {
            printf("Sent: %s\n", buffer);
        }

        xtimer_sleep(MSG_INTERVAL);
    }

    return 0;
}
