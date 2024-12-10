
#include <net/ipv6/addr.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <string.h>
#include <unistd.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define INTERVAL_SEC 5

int main(void)
{
    int sock;
    struct sockaddr_in server_addr;
    char buffer[10];

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        return -1;
    }

    // Set up server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        close(sock);
        return -1;
    }

    // Main loop to send device state every 5 seconds
    while (1) {
        // Prepare the message
        strncpy(buffer, DEVICE_STATE, sizeof(buffer) - 1);
        buffer[sizeof(buffer) - 1] = '\0';

        // Send the message
        if (sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            close(sock);
            return -1;
        }

        // Wait for 5 seconds
        sleep(INTERVAL_SEC);
    }

    // Close the socket
    close(sock);

    return 0;
}
