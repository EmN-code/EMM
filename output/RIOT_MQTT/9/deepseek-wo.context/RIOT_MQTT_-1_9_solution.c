
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>
#include <net/mqtt_client.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U) /* 5 seconds in microseconds */

static MQTTClient client;
static Network network;
static unsigned char sendbuf[256];
static unsigned char readbuf[256];

void connect_to_broker(void) {
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "riot_esp32";
    data.keepAliveInterval = 60;
    data.cleansession = 1;

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    int rc = NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    if (rc != 0) {
        printf("Failed to connect to broker\n");
        return;
    }

    rc = MQTTConnect(&client, &data);
    if (rc != 0) {
        printf("Failed to connect MQTT client\n");
        return;
    }

    printf("Connected to MQTT broker\n");
}

void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    int rc = MQTTPublish(&client, TOPIC, &message);
    if (rc != 0) {
        printf("Failed to publish message\n");
    } else {
        printf("Published: %s\n", PAYLOAD);
    }
}

int main(void) {
    connect_to_broker();

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
