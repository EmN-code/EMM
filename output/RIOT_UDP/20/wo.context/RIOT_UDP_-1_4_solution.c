#include "xtimer.h"
#include "thread.h"
#include "sock.h"
#include <string.h>
#include <stdio.h>

#define SERVER_ADDR      "47.102.103.1"
#define SERVER_PORT      20001
#define SEND_INTERVAL    (5U * US_PER_SEC)  // 5 seconds interval in microseconds

int main(void) {
    sock_udp_ep_t server = {
        .family = AF_INET,
        .port = SERVER_PORT
    };

    // converting IP address from string to network format
    inet_pton(AF_INET, SERVER_ADDR, &server.addr.ipv4);

    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);

    char *device_state = "work";
    while (1) {
        ssize_t res = sock_udp_send(&sock, device_state, strlen(device_state), &server);
        if (res < 0) {
            puts("Error sending device state");
        } else {
            puts("Device state sent successfully");
        }
        xtimer_sleep(SEND_INTERVAL / US_PER_SEC);  // convert back to seconds for sleep
    }

    return 0;
}
