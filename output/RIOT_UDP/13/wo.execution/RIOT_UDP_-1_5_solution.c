#include "paho_mqtt.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "mutex.h"
#include "mma8x5x_params.h"
#include "MQTTClient.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "event/thread.h"
#include "dht_params.h"
#include "arpa/inet.h"
#include "thread.h"
#include "sys/socket.h"
#include "net/emcute.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define INTERVAL 5

int main(void) {
    struct sockaddr_in server_addr;
    int sock;

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    // Configure server address struct
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    while (1) {
        // Send device state to server
        ssize_t sent_bytes = sendto(sock, DEVICE_STATE, sizeof(DEVICE_STATE), 0,
                                   (struct sockaddr *)&server_addr, sizeof(server_addr));
        if (sent_bytes < 0) {
            perror("sendto");
        }

        // Wait for the specified interval
        ztimer_sleep(ZTIMER_MSEC, INTERVAL * 1000);
    }

    return 0;
}