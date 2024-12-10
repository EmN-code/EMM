
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <net/mqtt_client.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_CLEAN_SESSION 1
#define MQTT_CLIENT_ID "riot_esp32"

#define TIMER_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static MQTTClient mqtt_client;
static Network mqtt_network;
static unsigned char sendbuf[256];
static unsigned char readbuf[256];

void mqtt_publish(void) {
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.dup = 0;
    message.id = 0;
    message.payload = (void*)MQTT_PAYLOAD;
    message.payloadlen = strlen(MQTT_PAYLOAD);

    int rc = MQTTPublish(&mqtt_client, MQTT_TOPIC, &message);
    if (rc != 0) {
        printf("MQTT publish failed with error code %d\n", rc);
    } else {
        printf("MQTT message published successfully\n");
    }
}

int main(void) {
    printf("Starting MQTT client on RIOT for ESP32\n");

    // Initialize the network
    NetworkInit(&mqtt_network);

    // Connect to the MQTT broker
    int rc = NetworkConnect(&mqtt_network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    // Initialize the MQTT client
    MQTTClientInit(&mqtt_client, &mqtt_network, MQTT_KEEPALIVE, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Set up the connection options
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = MQTT_CLIENT_ID;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = MQTT_CLEAN_SESSION;

    // Connect to the MQTT broker
    rc = MQTTConnect(&mqtt_client, &options);
    if (rc != 0) {
        printf("MQTT connect failed with error code %d\n", rc);
        return 1;
    }

    printf("Connected to MQTT broker\n");

    // Publish a message every 5 seconds
    while (1) {
        mqtt_publish();
        xtimer_usleep(TIMER_INTERVAL);
    }

    return 0;
}
