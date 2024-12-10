#include <xtimer.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/af.h>

#define SERVER_PORT 20001
#define INTERVAL (5 * US_PER_SEC)  // 5 seconds in microseconds
static sock_udp_ep_t remote;

int main(void) {
    char *server_ip = "47.102.103.1"; 
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, server_ip, &remote.addr.ipv4);

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return 1;
    }

    while (1) {
        char *msg = "Device State: work";
        if (sock_udp_send(&sock, msg, strlen(msg), &remote) < 0) {
            puts("Error: unable to send UDP message");
        } else {
            puts("Sent: Device State: work");
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}