#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/af.h>
#include <stdio.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define SEND_INTERVAL 5

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = UDP_SERVER_PORT };
    int res;

    // Convert the IP address from string format to binary format
    res = inet_pton(AF_INET, UDP_SERVER_IP, &remote.addr.ipv4);
    if (res <= 0) {
        if (res == 0) {
            puts("Invalid IP address format.");
        } else {
            puts("Error converting IP address.");
        }
        return 1;
    }

    // Create UDP socket
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP socket.");
        return 1;
    }

    while (1) {
        const char *device_state = "work or not work";
        // Send device state to the server
        if (sock_udp_send(&sock, device_state, strlen(device_state), &remote) < 0) {
            puts("Error sending data.");
        } else {
            puts("Data sent successfully.");
        }
        
        // Wait for 5 seconds before sending the next message
        xtimer_sleep(SEND_INTERVAL);
    }
    return 0;
}