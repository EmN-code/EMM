#include "xtimer.h"
#include "net/ipv6/addr.h"
#include <stdio.h>
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883

#define CLIENT_ID    "esp32_client"
#define TOPIC        "example/state"
#define STACK_SIZE   (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)
#define QOS          1
#define TIMEOUT      1000L

void publish_state(MQTTClient *client, const char *state) {
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.payload = (void*)state;
    message.payloadlen = strlen(state);

    int rc = MQTTPublish(client, TOPIC, &message);
    if (rc != SUCCESS) {
        printf("Failed to publish message, return code: %d\n", rc);
    }
    else {
        printf("Message published successfully: %s\n", state);
    }
}

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[100], readbuf[100];

    printf("MQTT example program\n");
    printf("Client ID: %s\n", CLIENT_ID);

    // Initialize the network stack
    NetworkInit(&network);

    // Establish network connection
    while (1) {
        int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connect failed
");
            xtimer_sleep(1);
            continue;
        }
        else {
            printf("Network connected successfully
");
            break;
        }
    }

    // Initialize MQTT client
    MQTTClientInit(&client, &network, TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to the MQTT broker
    while (1) {
        MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;
        connect_data.MQTTVersion = 3;
        connect_data.clientID.cstring = CLIENT_ID;
        int rc = MQTTConnect(&client, &connect_data);
        if (rc != SUCCESS) {
            printf("MQTT connect failed, rc=%d\n", rc);
            xtimer_sleep(1);
            continue;
        }
        else {
            printf("MQTT connected successfully
");
            break;
        }
    }

    // Publish messages every 5 seconds
    while (1) {
        publish_state(&client, "work");
        xtimer_sleep(5);
    }

    // Cleanup on exit
    return 0;
}