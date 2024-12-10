#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5U * US_PER_SEC)

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET6, .port = SERVER_PORT };
    sock_udp_t sock;
    char *device_state_work = "work";
    // Remove unused variable 'device_state_not_work' to fix warning
    // char *device_state_not_work = "not work";
    char *device_state = device_state_work;
    int res;

    res = sock_udp_create(&sock, NULL, NULL, 0);
    if (res < 0) {
        printf("Error initializing socket\n");
        return 1;
    }

    res = inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4);
    if (res == 0) {
        printf("Error: Could not convert the IP address\n");
        return 1;
    }

    while (1) {
        res = sock_udp_send(&sock, device_state, strlen(device_state), &server);
        if (res < 0) {
            printf("Error sending data\n");
        } else {
            printf("Sent: %s\n", device_state);
        }
        xtimer_sleep(INTERVAL);
    }

    return 0;
}