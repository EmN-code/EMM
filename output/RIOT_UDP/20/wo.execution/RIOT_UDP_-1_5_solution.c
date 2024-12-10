#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/af.h>
#include <net/sock/udp.h>
#include <msg.h>
#include <thread.h>
#include <time_units.h>
#include <mutex.h>
#include <kernel_defines.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <ztimer.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SLEEP_INTERVAL (5U * 1000U) // 5 seconds in milliseconds

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: unable to convert IP address\n");
        return -1;
    }

    char message[] = "work";

    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: unable to create UDP socket\n");
        return -1;
    }

    while (1) {
        ssize_t res = sock_udp_send(&sock, message, sizeof(message), &remote);
        if (res < 0) {
            printf("Error: unable to send message\n");
        } else {
            printf("Message sent: %s\n", message);
        }

        ztimer_sleep(ZTIMER_MSEC, SLEEP_INTERVAL);
    }

    return 0;
}
