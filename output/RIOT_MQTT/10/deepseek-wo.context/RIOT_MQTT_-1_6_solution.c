
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/sock/util.h>
#include <net/ipv4/addr.h>
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"
#define MQTT_KEEPALIVE 30
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static void network_connect(Network *n, const char *address, int port) {
    int result = n->connect(n, address, port);
    if (result != 0) {
        printf("Failed to connect to MQTT broker\n");
        return;
    }
    printf("Connected to MQTT broker\n");
}

static void network_disconnect(Network *n) {
    n->disconnect(n);
    printf("Disconnected from MQTT broker\n");
}

static int mqtt_publish(MQTTClient *client, const char *topic, const char *message) {
    MQTTMessage msg;
    msg.qos = 0;
    msg.retained = 0;
    msg.dup = 0;
    msg.payload = (void *)message;
    msg.payloadlen = strlen(message);

    return MQTTPublish(client, topic, &msg);
}

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[200], readbuf[200];
    int rc;

    // Initialize the network
    NetworkInit(&network);
    network_connect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);

    // Initialize the MQTT client
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.clientID.cstring = MQTT_CLIENT_ID;
    data.keepAliveInterval = MQTT_KEEPALIVE;
    data.cleansession = 1;

    rc = MQTTConnect(&client, &data);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker, return code %d\n", rc);
        network_disconnect(&network);
        return -1;
    }
    printf("MQTT connected\n");

    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        // Publish the message
        rc = mqtt_publish(&client, MQTT_TOPIC, MQTT_MESSAGE);
        if (rc != 0) {
            printf("Failed to publish message, return code %d\n", rc);
        } else {
            printf("Message published\n");
        }

        // Wait for 5 seconds
        xtimer_periodic_wakeup(&last_wakeup, PUBLISH_INTERVAL);
    }

    // Disconnect from the MQTT broker
    network_disconnect(&network);

    return 0;
}
