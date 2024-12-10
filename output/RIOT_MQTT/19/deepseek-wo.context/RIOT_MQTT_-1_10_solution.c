
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <net/mqtt_client.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 30
#define MQTT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define MQTT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static MQTTClient mqtt_client;
static Network network;
static unsigned char sendbuf[100];
static unsigned char readbuf[100];

void mqtt_publish(void) {
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.dup = 0;
    message.id = 0;
    message.payload = (void*)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    MQTTPublish(&mqtt_client, MQTT_TOPIC, &message);
}

int main(void) {
    printf("Starting MQTT client...\n");

    // Initialize network
    NetworkInit(&network);
    NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);

    // Initialize MQTT client
    MQTTClientInit(&mqtt_client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = 1;
    options.clientID.cstring = "riot_mqtt_client";

    int rc = MQTTConnect(&mqtt_client, &options);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker, return code %d\n", rc);
        return 1;
    }

    printf("Connected to MQTT broker\n");

    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }

    return 0;
}
