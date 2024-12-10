
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/mqtt.h>
#include <net/mqtt_client.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U) /* 5 seconds in microseconds */

static MQTTClient client;
static Network network;
static unsigned char sendbuf[2048];
static unsigned char readbuf[2048];

void mqtt_connect(void) {
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "riot_client";
    data.keepAliveInterval = 60;
    data.cleansession = 1;

    int rc = MQTTConnect(&client, &data);
    if (rc != 0) {
        printf("MQTT connect failed, return code %d\n", rc);
    } else {
        printf("MQTT connected\n");
    }
}

void mqtt_publish(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    int rc = MQTTPublish(&client, TOPIC, &message);
    if (rc != 0) {
        printf("MQTT publish failed, return code %d\n", rc);
    } else {
        printf("MQTT published\n");
    }
}

int main(void) {
    printf("Starting MQTT example\n");

    NetworkInit(&network);
    int rc = NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    if (rc != 0) {
        printf("Network connect failed, return code %d\n", rc);
        return 1;
    }

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    mqtt_connect();

    while (1) {
        mqtt_publish();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
