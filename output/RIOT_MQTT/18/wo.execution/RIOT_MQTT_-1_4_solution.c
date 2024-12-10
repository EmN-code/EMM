#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <esp_system.h>
#include <mutex.h>
#include <event/thread.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/mqttsn.h>
#include <paho_mqtt.h>
#include <net/emcute.h>
#include <MQTTClient.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "device/state"
#define MESSAGE "work"

void publish_state(int sock, struct sockaddr_in *broker_addr) {
    char msg[256];
    snprintf(msg, sizeof(msg), MESSAGE);
    sendto(sock, msg, sizeof(msg), 0, (struct sockaddr *)broker_addr, sizeof(*broker_addr));
}

int main() {
    int sock;
    struct sockaddr_in broker_addr;
    
    // Create a UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket creation failed");
        return -1;
    }
    
    // Set up the broker address struct
    memset(&broker_addr, 0, sizeof(broker_addr));
    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(BROKER_PORT);
    
    // Convert IP address from text to binary form
    if (inet_pton(AF_INET, BROKER_IP, &broker_addr.sin_addr) <= 0) {
        perror("inet_pton failed");
        return -1;
    }
    
    // Infinite loop to publish state every 5 seconds
    while (1) {
        publish_state(sock, &broker_addr);
        sleep(5);
    }

    return 0;
}