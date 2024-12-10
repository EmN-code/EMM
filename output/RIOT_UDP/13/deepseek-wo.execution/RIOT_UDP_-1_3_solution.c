
#include <net/emcute.h>
#include <net/gcoap.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <string.h>
#include <unistd.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define SEND_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

int main(void)
{
    int sock;
    struct sockaddr_in server_addr;
    char buffer[10];

    // Create a UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        puts("Error creating socket");
        return 1;
    }

    // Set up the server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        puts("Invalid address/ Address not supported");
        close(sock);
        return 1;
    }

    // Prepare the device state message
    strncpy(buffer, DEVICE_STATE, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    while (1) {
        // Send the device state to the server
        if (sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            puts("Failed to send message");
        } else {
            puts("Message sent");
        }

        // Wait for 5 seconds before sending the next message
        usleep(SEND_INTERVAL);
    }

    // Close the socket (this part is unreachable in this example)
    close(sock);

    return 0;
}
