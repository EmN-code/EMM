#include <ztimer.h>  // For timer functionality
#include <stdio.h>   // For printf function
#include <string.h>  // For memset function
#include <net/sock/udp.h> // For UDP socket functions

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL 5U // Send interval in seconds

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4);

    const char *state = "work";  // The device state to send
    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);

    while (1) {
        sock_udp_send(&sock, state, strlen(state), &server);
        printf("Sent state: %s\n", state);
        ztimer_sleep(ZTIMER_SEC, SEND_INTERVAL);
    }

    return 0;
}