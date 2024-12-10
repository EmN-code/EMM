#include <string.h> //needed for memset
#include <stdio.h> //needed for printf
#include <net/ipv4/addr.h> //needed for inet_pton
#include <xtimer.h>
#include <sock_udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL 5

int main(void) {
    // Setting up the server address
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4);

    // Creating the UDP socket
    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);

    const char *message = "Device state: work";
    while (1) {
        // Send the device state message to the server
        if (sock_udp_send(&sock, message, strlen(message), &server) < 0) {
            printf("Error sending message\n");
        } else {
            printf("Message sent: %s\n", message);
        }
        // Wait for the interval period
        xtimer_sleep(SEND_INTERVAL);
    }
    // Close socket (Note: unreachable in this example)
    sock_udp_close(&sock);
    return 0;
}