#include "esp_system.h"
#include "net/sock/util.h"
#include "thread.h"
#include "netinet/in.h"
#include "mutex.h"
#include "net/emcute.h"
#include "MQTTClient.h"
#include "arpa/inet.h"
#include "net/gcoap.h"
#include "net/af.h"
#include "event/thread.h"
#include "sys/socket.h"
#include "net/sock/udp.h"

// Define the remote server address and port
#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001

// Define the interval for sending the state
#define SEND_INTERVAL 5

// Function to send the device state to the specified server address
void send_device_state(int sock, struct sockaddr_in *server_addr) {
    const char *state = "work or not work";
    if (sendto(sock, state, strlen(state), 0, (struct sockaddr *)server_addr, sizeof(*server_addr)) < 0) {
        printf("Failed to send data\n");
    } else {
        printf("Device state sent successfully\n");
    }
}

int main(void) {
    int sock;
    struct sockaddr_in server_addr;
    int attempts;

    // Create a socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        printf("Failed to create socket\n");
        return -1;
    }

    // Setup the server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr) <= 0) {
        printf("Invalid address or Address not supported\n");
        return -1;
    }

    // Loop indefinitely, sending the device state every SEND_INTERVAL seconds
    while (1) {
        attempts = 0;
        while (1) {
            send_device_state(sock, &server_addr);
            attempts++;
            if (attempts >= 3) {
                printf("Could not send after multiple attempts\n");
                break;
            }
            sleep(1);  // Try again after one second if sending failed
        }
        sleep(SEND_INTERVAL);
    }

    // Close the socket (although we won't reach here in the current program structure)
    close(sock);

    return 0;
}