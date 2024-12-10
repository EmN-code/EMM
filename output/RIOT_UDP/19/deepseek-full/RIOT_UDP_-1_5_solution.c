
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <mutex.h>
#include <mma8x5x_params.h>
#include <net/sock/udp.h>
#include <dht_params.h>
#include <event/thread.h>
#include <esp_wifi.h>
#include <net/gcoap.h>
#include <MQTTClient.h>
#include <esp_system.h>
#include <net/emcute.h>
#include <thread.h>
#include <net/sock/util.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define SEND_INTERVAL (5 * 1000000) // 5 seconds in microseconds

int main(void)
{
    int sock;
    struct sockaddr_in server_addr;
    char buffer[100];

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        puts("Failed to create socket");
        return 1;
    }

    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        puts("Invalid address/ Address not supported");
        close(sock);
        return 1;
    }

    while (1) {
        // Prepare the message
        snprintf(buffer, sizeof(buffer), "Device state: %s", DEVICE_STATE);

        // Send the message
        if (sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            puts("Failed to send message");
        } else {
            puts("Message sent successfully");
        }

        // Wait for 5 seconds before sending the next message
        usleep(SEND_INTERVAL);
    }

    // Close the socket
    close(sock);
    return 0;
}
