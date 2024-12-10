#include <xtimer.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/af.h>
#include <stdio.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5U * US_PER_SEC) // 5 seconds interval

int main(void) {
    // Define socket and endpoint
    sock_udp_t sock;
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };

    // Convert IP address from string to binary form
    if (inet_pton(AF_INET, SERVER_ADDR, &server.addr.ipv4) != 1) {
        printf("Error: Unable to parse server address\n");
        return -1;
    }
   
    // Create UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: Unable to create UDP socket\n");
        return -1;
    }

    // Message to send
    char work_state[] = "work";

    // Loop to send message every five seconds
    while (1) {
        ssize_t res = sock_udp_send(&sock, work_state, sizeof(work_state), &server);
        if (res < 0) {
            printf("Error sending UDP packet\n");
        } else {
            printf("Sent 'work_state' to server\n");
        }
        xtimer_sleep(INTERVAL);
    }

    // Close the socket
    sock_udp_close(&sock);
    return 0;
}