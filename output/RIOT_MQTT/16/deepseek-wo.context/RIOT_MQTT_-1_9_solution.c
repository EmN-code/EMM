
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS QOS0
#define MQTT_KEEPALIVE 60
#define MQTT_YIELD_TIME 1000
#define PUBLISH_INTERVAL 5000000

static MQTTClient mqtt_client;
static Network mqtt_network;
static unsigned char sendbuf[200];
static unsigned char readbuf[200];

void mqtt_connect(void) {
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = 1;
    options.MQTTVersion = 3;
    options.clientID.cstring = "riot_esp32";

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
    message.dup = 0;
    message.payload = (void*)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    int rc = MQTTPublish(&mqtt_client, MQTT_TOPIC, &message);
    if (rc != 0) {
        printf("MQTT publish failed, return code %d\n", rc);
    } else {
        printf("MQTT message published\n");
    }
}

void mqtt_yield(void) {
    int rc = MQTTYield(&mqtt_client, MQTT_YIELD_TIME);
    if (rc != 0) {
        printf("MQTT yield failed, return code %d\n", rc);
    }
}

int main(void) {
    printf("Starting MQTT example\n");

    // Initialize network
    int rc = NetworkInit(&mqtt_network);
    if (rc != 0) {
        printf("Network initialization failed, return code %d\n", rc);
        return 1;
    }

    // Connect to MQTT broker
    rc = NetworkConnect(&mqtt_network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Network connect failed, return code %d\n", rc);
        return 1;
    }

    // Initialize MQTT client
    MQTTClientInit(&mqtt_client, &mqtt_network, 30000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to MQTT broker
    mqtt_connect();

    // Main loop
    while (1) {
        mqtt_publish();
        mqtt_yield();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
