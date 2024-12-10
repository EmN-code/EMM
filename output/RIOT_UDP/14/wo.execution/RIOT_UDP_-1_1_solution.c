#include <xtimer.h>
#include <net/sock/udp.h>
#include <stdio.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL 5
#define BUF_SIZE 64

static sock_udp_t sock;
static sock_udp_ep_t server;

static const char *get_device_state(void) {
    // This function simulates checking the device state
    // In a real application, you would check the actual device state here
    // For now, we will simulate that the device is always "working"
    return "work";
}

int main(void) {
    server.family = AF_INET6;
    server.port = SERVER_PORT;
    if (net_if_ipv4_addr_set_all(0, IPV4_ADDR_ANY, &server.addr.ipv4) != 0) {
        puts("Error setting server address\n");
        return 1;
    }

    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP sock\n");
        return 1;
    }

    while (1) {
        char buffer[BUF_SIZE];
        const char *device_state = get_device_state();

        snprintf(buffer, BUF_SIZE, "Device state: %s", device_state);

        if (sock_udp_send(&sock, buffer, strlen(buffer), &server) < 0) {
            puts("Error sending UDP packet\n");
        } else {
            printf("Sent UDP packet: %s\n", buffer);
        }

        xtimer_sleep(INTERVAL);
    }

    return 0;
}