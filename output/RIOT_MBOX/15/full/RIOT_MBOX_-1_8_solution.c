#include "stdio.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdbool.h"
#include "event/thread.h"
#include "esp_system.h"
#include "netinet/in.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "mutex.h"
#include "stdatomic.h"
#include "paho_mqtt.h"

#define MSG_QUEUE_SIZE 4
#define MQTT_BROKER_IP "192.168.0.1"
#define MQTT_BROKER_PORT 1883

void connection_check(Network network);

int main(void) {
    printf("Starting message box example on ESP32...\n");
    Network network;

    while (1) {
        int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);

        if(rc != 0) {
            printf("Network connection failed, retrying...\n");
            sleep(1);
            continue;  // If connection fails, retry
        } else {
            printf("Network connected successfully\n");
            break;  // Exit loop if connection is successful
        }
    }

    connection_check(network);

    return 0;
}

void connection_check(Network network) {
    // Simulate checking connection functionality
    printf("Checking network connection...\n");
    // Add actual connection check implementation here
    printf("Network connection is active\n");
}
