#include <xtimer.h>
#include <string.h>
#include <net/sock/udp.h>

#define PERIOD 5U
#define SERVER_PORT 20001
#define SERVER_IP "47.102.103.1"

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    int res;

    // Convert the IP address from a string to a network representation
    res = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (res <= 0) {
        puts("ERROR: Unable to convert IP address");
        return 1;
    }

    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("ERROR: Unable to create socket");
        return 1;
    }

    while (1) {
        const char *message = "Device state: work or not work";
        if (sock_udp_send(&sock, message, strlen(message), &remote) < 0) {
            puts("ERROR: Unable to send message");
        } else {
            puts("Message sent successfully");
        }
        xtimer_sleep(PERIOD);
    }

    return 0;
}
