#include <string.h>  // For strcmp, memset
#include <stdio.h>    // For printf
#include <stdlib.h>   // For exit
#include <net/af.h>   // For AF_INET
#include <net/sock/udp.h>  // For socket functions
#include <xtimer.h>   // For xtimer_sleep
#include <thread.h>   // For thread control

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001

// Function to send device state
void send_device_state(char *state) {
    sock_udp_ep_t server = {
        .family = AF_INET,
        .port = SERVER_PORT
    };

    if (ipv4_addr_from_str((ipv4_addr_t *)&server.addr.ipv4, SERVER_ADDR) == NULL) {
        printf("Error: unable to parse server address\n");
        exit(EXIT_FAILURE);
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: unable to create socket\n");
        exit(EXIT_FAILURE);
    }

    int res = sock_udp_send(&sock, state, strlen(state), &server);
    if (res >= 0) {
        printf("Sent: %s\n", state);
    } else {
        printf("Error sending state\n");
    }

    sock_udp_close(&sock);
}

int main(void) {
    const char *state = "work";
    while (1) {
        send_device_state((char *)state);
        xtimer_sleep(5);  // Sleep for 5 seconds
    }
    return 0;
}