
#include <net/emcute.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/gcoap.h>
#include <sys/socket.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/sock.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define INTERVAL_MS (5 * 1000)

int main(void)
{
    int sock;
    struct sockaddr_in server_addr;
    char buffer[10];

    // Create a UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        puts("Failed to create socket");
        return 1;
    }

    // Set up the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        puts("Invalid address/ Address not supported");
        return 1;
    }

    // Prepare the device state message
    strcpy(buffer, DEVICE_STATE);

    while (1) {
        // Send the device state to the server
        if (sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            puts("Failed to send message");
        } else {
            puts("Message sent successfully");
        }

        // Wait for 5 seconds before sending the next message
        xtimer_usleep(INTERVAL_MS * 1000);
    }

    // Close the socket (this part will never be reached in this example)
    close(sock);

    return 0;
}
