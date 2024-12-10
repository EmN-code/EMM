#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <pthread.h>
#include <unistd.h>
#include <paho.mqtt.embedded-c/MQTTClient.h>
#include <sys/time.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_CLIENT_ID "esp32_mqtt_client"

volatile int running = 1;  // keep the loop running

void handle_sigint(int sig) {
    running = 0;
}

void* mqtt_publish_task(void* args) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[80], readbuf[80];
    int rc = 0;

    NetworkInit(&network);
    rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != 0) {
        fprintf(stderr, "Failed to connect to broker\n");
        pthread_exit(NULL);
    }

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = MQTT_CLIENT_ID;

    rc = MQTTConnect(&client, &data);
    if (rc != 0) {
        fprintf(stderr, "Failed to connect to MQTT broker\n");
        pthread_exit(NULL);
    }

    while (running) {
        MQTTMessage message;
        char *state_message = "work";

        memset(&message, 0, sizeof(message));
        message.qos = QOS0;
        message.retained = 0;
        message.payload = (void*)state_message;
        message.payloadlen = strlen(state_message);

        rc = MQTTPublish(&client, MQTT_TOPIC, &message);
        if (rc != 0) {
            fprintf(stderr, "Failed to publish message\n");
        }

        sleep(5);  // Publish every 5 seconds
    }

    MQTTDisconnect(&client);
    NetworkDisconnect(&network);
    pthread_exit(NULL);
}

int main() {
    signal(SIGINT, handle_sigint);

    pthread_t mqtt_thread;
    if (pthread_create(&mqtt_thread, NULL, mqtt_publish_task, NULL)) {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }

    pthread_join(mqtt_thread, NULL);
    return 0;
}