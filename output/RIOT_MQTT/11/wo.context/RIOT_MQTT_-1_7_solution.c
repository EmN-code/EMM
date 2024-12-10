#include <ztimer.h>  // for using ztimer for timing functions
#include <thread.h>  // for thread operations, which might be needed for MQTT
#include <mutex.h>   // for using mutexes, if needed by MQTT
#include <net/sock.h> // for networking socket operations
#include <net/af.h> // for address family definitions (e.g. AF_INET)
#include <stdlib.h> // for standard library functions
#include <string.h> // for string operations
#include "MQTTClient.h" // last include, for MQTT functionality

#define ADDRESS_IP "47.102.103.1"
#define PORT 1883
#define CLIENT_ID "RIOT_ESP32_Client"
#define TOPIC "device/state"
#define PAYLOAD "work"
#define QOS 1

// Function to establish network connection
int NetworkConnect(Network *n, char* address_ip, int port_number) {
    // Implementation to connect to the broker address and port
    // This usually involves using sockets to establish a TCP connection

    // Placeholder for socket connection success
    return 0; 
}

// Main function
int main(void) {
    // Setup the MQTT client
    unsigned char sendbuf[80];
    unsigned char readbuf[80];
    Network network;
    MQTTClient client;

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    
    // Connect to MQTT broker
    int rc = NetworkConnect(&network, ADDRESS_IP, PORT);
    if(rc != 0) {
        printf("Failed to connect to network.\n");
        return -1;
    }

    // MQTT connect data
    MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;
    connect_data.MQTTVersion = 3;
    connect_data.clientID.cstring = CLIENT_ID;
    rc = MQTTConnect(&client, &connect_data);

    if(rc != SUCCESS) {
        printf("Failed to connect to MQTT broker.\n");
        return -2;
    }

    while (1) {
        // Publishing work state to MQTT Broker
        MQTTMessage message;
        message.qos = QOS;
        message.retained = 0;
        message.payload = (void *)PAYLOAD;
        message.payloadlen = strlen(PAYLOAD);
        rc = MQTTPublish(&client, TOPIC, &message);
        if(rc != SUCCESS) {
            printf("Failed to publish message.\n");
        }

        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    // Disconnect
    MQTTDisconnect(&client);
    NetworkDisconnect(&network);

    return 0;
}