#include "net/sock/util.h"
#include "MQTTClient.h"
#include "net/gcoap.h"
#include "netinet/in.h"
#include "paho_mqtt.h"
#include "esp_system.h"
#include "dht_params.h"
#include "arpa/inet.h"
#include "kernel_defines.h"
#include "esp_wifi.h"
#include "sys/socket.h"
#include "net/sock/udp.h"
#include "mutex.h"
#include "net/emcute.h"
#include "net/af.h"
#include "thread.h"
#include "event/thread.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001

int main(void) {
    // Set up UDP socket
    int sock;
    struct sockaddr_in server_addr;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        printf("Failed to create socket.\n");
        return -1;
    }

    // Initialize server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // Define the message to send
    const char* device_state = "work or not work";

    while (1) {
        // Send the message
        ssize_t sent_bytes = sendto(sock, device_state, strlen(device_state), 0,
                                    (struct sockaddr *)&server_addr, sizeof(server_addr));
        if (sent_bytes < 0) {
            printf("Failed to send data.\n");
        } else {
            printf("Data sent successfully.\n");
        }

        // Wait for 5 seconds before sending again
        sleep(5);
    }

    // Close the socket
    close(sock);
    return 0;
}