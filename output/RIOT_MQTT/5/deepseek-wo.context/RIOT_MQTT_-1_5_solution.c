
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>
#include <net/mqtt_client.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS QOS0
#define MQTT_KEEPALIVE 60
#define MQTT_CLIENT_ID "esp32_client"

static MQTTClient mqtt_client;
static Network mqtt_network;
static unsigned char sendbuf[256];
static unsigned char readbuf[256];

void mqtt_connect(void) {
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.MQTTVersion = 4;
    options.clientID.cstring = MQTT_CLIENT_ID;
    options.cleansession = 1;

    int rc = MQTTConnect(&mqtt_client, &options);
    if (rc != 0) {
        printf("MQTT connect failed, return code %d\n", rc);
    } else {
        printf("MQTT connected\n");
    }
}

void mqtt_publish(void) {
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = 0;
    message.payload = (void*)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    int rc = MQTTPublish(&mqtt_client, MQTT_TOPIC, &message);
    if (rc != 0) {
        printf("MQTT publish failed, return code %d\n", rc);
    } else {
        printf("MQTT message published\n");
    }
}

int main(void) {
    printf("Starting MQTT client example\n");

    // Initialize the network
    NetworkInit(&mqtt_network);

    // Connect to the MQTT broker
    int rc = NetworkConnect(&mqtt_network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Network connect failed, return code %d\n", rc);
        return 1;
    }

    // Initialize the MQTT client
    MQTTClientInit(&mqtt_client, &mqtt_network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to the MQTT broker
    mqtt_connect();

    // Publish a message every 5 seconds
    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }

    return 0;
}
