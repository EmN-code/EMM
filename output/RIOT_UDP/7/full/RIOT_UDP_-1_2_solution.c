#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sys/socket.h"
#include "net/af.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL 5

int main(void)
{
    int sock;
    struct sockaddr_in server_addr;
    const char *msg = "work or not work";
    int rc;

    // Create UDP socket
    while (1) {
        sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (sock < 0) {
            puts("Error creating socket");
            continue;
        } else {
            puts("Socket created");
            break;
        }
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);

    while (1) {
        // Attempt to send the message
        rc = sendto(sock, msg, strlen(msg), 0,
                    (struct sockaddr *)&server_addr, sizeof(server_addr));
        // Check if the message was sent successfully
        if (rc < 0) {
            puts("Error sending message");
        } else {
            printf("Message sent to %s:%d\n", SERVER_ADDR, SERVER_PORT);
        }
        // Wait for 5 seconds before sending the next message
        sleep(INTERVAL);
    }

    // Close the socket (unreachable in this example, but good practice)
    close(sock);
    return 0;
}