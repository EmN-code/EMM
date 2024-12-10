#include <ztimer.h> // Timing functions
#include <xtimer.h> // Timer related functions
#include <thread.h> // For threading functionalities
#include <stdio.h> // For printf functionality
#include <arpa/inet.h> // For network related conversions
#include <stdlib.h> // For general utilities
#include <sys/socket.h> // Socket functionalities
#include <netinet/in.h> // Internet Protocol family
#include "MQTTClient.h" // Contains MQTT Client functionalities

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_PUBLISH_TOPIC "device/state"

static char thread_stack[THREAD_STACKSIZE_DEFAULT];

void *mqtt_thread(void *arg) {
    (void)arg;
    MQTTClient client;
    Network network;
    unsigned char sendbuf[80], readbuf[80];
    int rc = 0;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "RIOTClient";

    while (1) {
        NetworkInit(&network); // Initialize network structures
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if(rc != 0){
            printf("Network connection failed. Retrying in 1 second...\n");
            xtimer_sleep(1);
            continue;
        }
        printf("Connected to network successfully.\n");
        
        MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
        rc = MQTTConnect(&client, &connectData);
        if (rc != 0) {
            printf("MQTT connection failed. Retrying in 1 second...\n");
            NetworkDisconnect(&network);
            xtimer_sleep(1);
            continue;
        }
        printf("MQTT Connected successfully.\n");

        MQTTMessage message;
        message.qos = QOS1;
        message.retained = 0;
        message.payload = (void*)"work or not work";
        message.payloadlen = strlen((char*)message.payload);

        while (1) {
            rc = MQTTPublish(&client, MQTT_PUBLISH_TOPIC, &message);
            if(rc != 0){
                printf("Failed to publish message. Retrying in 5 second...\n");
                break;
            }
            printf("Message published successfully: %s\n", (char*)message.payload);
            xtimer_sleep(5);
        }

        MQTTDisconnect(&client);
        NetworkDisconnect(&network);
    }
    return NULL;
}

int main(void) {
    printf("MQTT Client starting...\n");
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1, 0, mqtt_thread, NULL, "MQTT Thread");
    return 0;
}
