#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>

#define SERVER_PORT    (20001)
#define TIME_INTERVAL  (5000000U) // 5 seconds in microseconds

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET, .addr.ipv4.u32 = 0, .port = SERVER_PORT };
    ip_addr_from_str((ip_addr_t *)&server.addr, "47.102.103.1");

    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;

    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        printf("Error initializing UDP
");
        return -1;
    }

    bool device_working = true;

    while (1) {
        char message[20];
        if (device_working) {
            snprintf(message, sizeof(message), "work");
        } else {
            snprintf(message, sizeof(message), "not work");
        }

        if (sock_udp_send(&sock, message, strlen(message), &server) < 0) {
            printf("Error sending UDP packet
");
        }
        printf("Message sent: %s
", message);

        device_working = !device_working;  // Simulate device state change
        xtimer_usleep(TIME_INTERVAL);
    }

    return 0;
}