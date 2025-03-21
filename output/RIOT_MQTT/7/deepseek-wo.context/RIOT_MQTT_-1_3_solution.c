
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/mqtt_client.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_CLIENT_ID "esp32_client"
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static MQTTClient mqtt_client;
static Network mqtt_network;
static unsigned char sendbuf[200];
static unsigned char readbuf[200];

void mqtt_connect(void) {
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = MQTT_CLIENT_ID;
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    int rc = MQTTConnect(&mqtt_client, &options);
    if (rc != 0) {
        printf("MQTT connect failed with error code %d\n", rc);
    } else {
        printf("MQTT connected successfully\n");
    }
}

void mqtt_publish(void) {
    MQTTMessage message;
    message.qos = 0;
    message.retained = 0;
    message.payload = (void*)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    int rc = MQTTPublish(&mqtt_client, MQTT_TOPIC, &message);
    if (rc != 0) {
        printf("MQTT publish failed with error code %d\n", rc);
    } else {
        printf("MQTT message published successfully\n");
    }
}

int main(void) {
    printf("Starting MQTT client on ESP32\n");

    // Initialize MQTT client
    MQTTClientInit(&mqtt_client, &mqtt_network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to MQTT broker
    int rc = NetworkConnect(&mqtt_network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Network connection failed with error code %d\n", rc);
        return 1;
    }

    mqtt_connect();

    // Publish state every 5 seconds
    while (1) {
        mqtt_publish();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
