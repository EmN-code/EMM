
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define SEND_INTERVAL 5

int main(void)
{
    int sock;
    struct sockaddr_in server_addr;
    char buffer[100];

    // Create a UDP socket
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        return -1;
    }

    // Set server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        return -1;
    }

    while (1) {
        // Prepare the message
        snprintf(buffer, sizeof(buffer), "Device state: %s", DEVICE_STATE);

        // Send the message to the server
        if (sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            perror("Sendto failed");
            return -1;
        }

        printf("Message sent: %s\n", buffer);

        // Wait for 5 seconds before sending the next message
        sleep(SEND_INTERVAL);
    }

    // Close the socket
    close(sock);

    return 0;
}
