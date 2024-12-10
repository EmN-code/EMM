#include <xtimer.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>

#define UDP_SERVER_ADDRESS "47.102.103.1"
#define UDP_SERVER_PORT    20001
#define SEND_INTERVAL      (5U * US_PER_SEC)

static void send_udp_message(sock_udp_ep_t *remote) {
    static const char *message = "Device State: work";
    ssize_t res = sock_udp_send(NULL, message, strlen(message), remote);
    if (res < 0) {
        printf("Error sending message: %d\n", (int)res);
    }
    else{
        printf("Message sent: %s\n", message);
    }
}

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET6, .port = UDP_SERVER_PORT };
    inet_pton(AF_INET6, UDP_SERVER_ADDRESS, &remote.addr.ipv6);

    printf("UDP client setup to %s:%d\n", UDP_SERVER_ADDRESS, UDP_SERVER_PORT);

    while (1) {
        send_udp_message(&remote);
        xtimer_sleep(SEND_INTERVAL / US_PER_SEC);
    }
    return 0;
}