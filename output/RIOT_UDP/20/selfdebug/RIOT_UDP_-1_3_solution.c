#include <xtimer.h>
#include <net/sock/udp.h>
#include <stdio.h>
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5U * US_PER_SEC) // 5 seconds in microseconds

static const char *device_state = "work"; // Change to "not work" as needed

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET6, .port = SERVER_PORT };
    size_t msg_len;
    uint8_t buf[64];
    sock_udp_t sock;
    server.addr.ipv4[0] = 47;
    server.addr.ipv4[1] = 102;
    server.addr.ipv4[2] = 103;
    server.addr.ipv4[3] = 1;

    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error creating UDP socket\n");
        return 1;
    }

    msg_len = snprintf((char *)buf, sizeof(buf), "%s", device_state);

    while (1) {
        if (sock_udp_send(&sock, buf, msg_len, &server) < 0) {
            printf("Error sending UDP message\n");
        } else {
            printf("UDP message sent: %s\n", buf);
        }
        xtimer_sleep(SEND_INTERVAL / US_PER_SEC);
    }

    return 0;
}