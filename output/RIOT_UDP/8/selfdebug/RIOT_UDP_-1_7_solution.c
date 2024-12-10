// Description: This program for the ESP32 running RIOT OS connects to a UDP server at the specified IP and port
// and sends the device's state ("work" or "not work") every five seconds.

// Necessary includes in descending alphabetical order
#include "xtimer.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"
#include "stdio.h"
#include "string.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE "work"
#define MESSAGE_INTERVAL (5U * US_PER_SEC)  // 5 seconds

int main(void) {
    // Socket and endpoint structures
    sock_udp_t sock;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    // Convert the server address from string to network byte order (byte array)
    if(inet_pton(AF_INET, SERVER_ADDR, &remote.addr.ipv4) != 1) {
        printf("Error: Unable to convert IP address\n");
        return -1;
    }

    // Create a UDP socket
    if(sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: Unable to create UDP socket\n");
        return -1;
    }

    while (1) {
        // Send the message to the server
        if(sock_udp_send(&sock, MESSAGE, strlen(MESSAGE), &remote) < 0) {
            printf("Error: Unable to send message\n");
        }
        else {
            printf("Message sent: %s\n", MESSAGE);
        }

        // Wait for the specified interval
        xtimer_sleep(MESSAGE_INTERVAL / US_PER_SEC);
    }

    // Close the socket (though this will never be reached in the current program)
    sock_udp_close(&sock);

    return 0;
}
