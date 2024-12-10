
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <net/ipv6/addr.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define BUFFER_SIZE 20
#define DELAY_MS 5000000

int main(void) {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Create a UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        perror("socket creation failed");
        return 1;
    }

    // Set up the server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("inet_pton failed");
        return 1;
    }

    // Main loop to send device state every 5 seconds
    while (1) {
        // Simulate device state (work or not work)
        const char *state = "work"; // or "not work"

        // Copy state to buffer
        strncpy(buffer, state, BUFFER_SIZE);

        // Send the state to the server
        if (sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            perror("sendto failed");
            return 1;
        }

        // Delay for 5 seconds
        xtimer_usleep(DELAY_MS);
    }

    // Close the socket
    close(sockfd);

    return 0;
}
