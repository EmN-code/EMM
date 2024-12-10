
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>
#include <net/mqtt_client.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define KEEP_ALIVE 60

static MQTTClient mqtt_client;
static Network network;
static unsigned char sendbuf[2048];
static unsigned char readbuf[2048];

void mqtt_connect(void) {
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "riot_esp32";
    data.keepAliveInterval = KEEP_ALIVE;
    data.cleansession = 1;

    int rc = MQTTConnect(&mqtt_client, &data);
    if (rc != 0) {
        printf("MQTT connect failed, return code %d\n", rc);
    } else {
        printf("MQTT connected\n");
    }
}

void mqtt_publish(void) {
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    int rc = MQTTPublish(&mqtt_client, TOPIC, &message);
    if (rc != 0) {
        printf("MQTT publish failed, return code %d\n", rc);
    } else {
        printf("MQTT message published\n");
    }
}

int main(void) {
    printf("Starting MQTT example\n");

    // Initialize network
    NetworkInit(&network);
    int rc = NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    if (rc != 0) {
        printf("Network connect failed, return code %d\n", rc);
        return 1;
    }

    // Initialize MQTT client
    MQTTClientInit(&mqtt_client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to MQTT broker
    mqtt_connect();

    // Publish state every 5 seconds
    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }

    return 0;
}
