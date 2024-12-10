#include <zephyr/net/socket.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/coap.h>
#include <unistd.h>
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define BUFLEN 64
#define SLEEP_TIME 5

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char message[] = "work";
    char buffer[BUFLEN];

    // Creating socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        perror("Failed to create socket");
        return -1;
    }

    // Setting up server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    while (1) {
        // Sending message to server
        if (sendto(sock, message, strlen(message), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            perror("Failed to send message");
        }
        
        // Wait for 5 seconds
        sleep(SLEEP_TIME);
    }

    // Closing socket (though this will never reach in current code)
    close(sock);
    return 0;
}