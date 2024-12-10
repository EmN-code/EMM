#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/af.h>
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1; // Exit with error
    }

    while (1) {
        if (sock_udp_send(&sock, MESSAGE, strlen(MESSAGE), &remote) < 0) {
            puts("Error sending data");
        }
        else {
            puts("Data sent successfully");
        }
        xtimer_usleep(INTERVAL);
    }

    sock_udp_close(&sock);  // Will actually never be reached in this loop
    return 0;
}